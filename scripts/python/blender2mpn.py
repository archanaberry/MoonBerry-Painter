# Export to MPN instruction format.
# Paste to Blender Text Editor, Run Script.
# Selection: change out_mpn, out_mpzc, scale, samples_per_bezier, projection

import bpy
import math
import mathutils
from mathutils import Vector
import gzip
import os
from mathutils.geometry import interpolate_bezier

# ---------- CONFIG ----------
out_mpn = bpy.path.abspath("//export.mpn")    # path relatif ke .blend file dir, atau absolute
out_mpzc = bpy.path.abspath("//export.mpzc")  # gzipped compiled version
scale = 1.0            # skala multiplier
samples_per_bezier = 12
projection = 'XY'      # 'XY', 'XZ', 'YZ', or 'CAMERA' (camera requires active camera)
include_grease_pencil = True
# ----------------------------

def project_co(co, obj_matrix):
    """Project a world-space Vector to 2D tuple per `projection`."""
    w = obj_matrix @ co
    if projection == 'XY':
        return (w.x * scale, w.y * scale)
    elif projection == 'XZ':
        return (w.x * scale, w.z * scale)
    elif projection == 'YZ':
        return (w.y * scale, w.z * scale)
    elif projection == 'CAMERA':
        cam = bpy.context.scene.camera
        if not cam:
            return (w.x * scale, w.y * scale)
        co_ndc = world_to_camera_view(bpy.context.scene, cam, obj_matrix @ co)
        # NDC -> screen coords (0..1)
        return (co_ndc.x * scale, co_ndc.y * scale)
    else:
        return (w.x * scale, w.y * scale)

def world_to_camera_view(scene, cam, coord):
    """Return normalized device coordinates (0..1) for point in world space relative to camera."""
    # from Blender docs
    co_local = cam.matrix_world.normalized().inverted() @ coord
    z = -co_local.z
    frame = [-v for v in cam.data.view_frame(scene=scene)[:3]]
    min_x = min([v.x for v in frame])
    max_x = max([v.x for v in frame])
    min_y = min([v.y for v in frame])
    max_y = max([v.y for v in frame])
    x = (co_local.x - min_x) / (max_x - min_x)
    y = (co_local.y - min_y) / (max_y - min_y)
    return Vector((x, y, z))

def write_mpn_header(f):
    f.write("; exported by Blender -> MoonBerry Painter exporter\n")
    f.write("F 1000\n")                # default feedrate
    f.write("DEFAULT_COLOR black\n")
    f.write("\n")

def emit_move(f, x, y):
    f.write(f"MOVE X{format(x, '.4f')} Y{format(y, '.4f')}\n")

def emit_draw(f, x, y):
    f.write(f"DRAW X{format(x, '.4f')} Y{format(y, '.4f')}\n")

def export_curve(obj, f):
    curve = obj.data
    m = obj.matrix_world
    # iterate splines
    for spline in curve.splines:
        if spline.type == 'BEZIER':
            bez_pts = spline.bezier_points
            if len(bez_pts) == 0:
                continue
            # iterate segments p -> q
            first = True
            for i in range(len(bez_pts)):
                p = bez_pts[i]
                q = bez_pts[(i + 1) % len(bez_pts)] if spline.use_cyclic_u else (bez_pts[i+1] if i+1 < len(bez_pts) else None)
                if q is None:
                    break
                p0 = m @ p.co
                p1 = m @ p.handle_right
                p2 = m @ q.handle_left
                p3 = m @ q.co
                samples = interpolate_bezier(p0, p1, p2, p3, samples_per_bezier)
                # samples is list of Vector points including endpoints
                if len(samples) == 0:
                    continue
                # emit move to first sample if first segment overall
                if first:
                    x0, y0 = project_co(samples[0], MatrixIdentity())
                    emit_move(f, x0, y0)
                    first = False
                for s in samples[1:]:
                    x, y = project_co(s, MatrixIdentity())
                    emit_draw(f, x, y)
            # if closed and not already closing, ensure close
            if spline.use_cyclic_u:
                f.write("CLOSE\n")
        else:
            # POLY / NURBS: sample points
            pts = spline.points if hasattr(spline, "points") else None
            if pts:
                coords = []
                for p in pts:
                    world = m @ Vector((p.co.x, p.co.y, p.co.z))
                    coords.append(project_co(world, MatrixIdentity()))
                if len(coords) >= 1:
                    emit_move(f, coords[0][0], coords[0][1])
                    for (x,y) in coords[1:]:
                        emit_draw(f, x, y)
                    if spline.use_cyclic_u:
                        f.write("CLOSE\n")

def MatrixIdentity():
    return mathutils.Matrix.Identity(4)

def export_mesh(obj, f):
    mesh = obj.data
    m = obj.matrix_world
    # ensure mesh has up-to-date evaluated coordinates
    depsgraph = bpy.context.evaluated_depsgraph_get()
    eval_obj = obj.evaluated_get(depsgraph)
    eval_mesh = eval_obj.to_mesh()
    vert_co = [m @ v.co for v in eval_mesh.vertices]
    # export each edge as a move-draw (may produce duplicates)
    for e in eval_mesh.edges:
        v1 = vert_co[e.vertices[0]]
        v2 = vert_co[e.vertices[1]]
        x1,y1 = project_co(v1, MatrixIdentity())
        x2,y2 = project_co(v2, MatrixIdentity())
        emit_move(f, x1, y1)
        emit_draw(f, x2, y2)
    # free the temporary mesh
    eval_obj.to_mesh_clear()

def export_grease_pencil(obj, f):
    # iterate frames/layers/strokes
    if not hasattr(obj.data, "layers"):
        return
    for layer in obj.data.layers:
        for frame in layer.frames:
            for stroke in frame.strokes:
                pts = stroke.points
                if len(pts) == 0:
                    continue
                first = True
                for p in pts:
                    co = obj.matrix_world @ Vector((p.co.x, p.co.y, p.co.z))
                    x,y = project_co(co, MatrixIdentity())
                    if first:
                        emit_move(f, x, y)
                        first = False
                    else:
                        emit_draw(f, x, y)

def export_selected_to_mpn(outpath=out_mpn, compress_out=out_mpzc):
    objs = bpy.context.selected_objects
    if not objs:
        print("No selected objects. Select curves/meshes/grease pencil objects.")
        return
    # write mpn
    with open(outpath, "w", encoding="utf-8") as f:
        write_mpn_header(f)
        for obj in objs:
            print("Exporting:", obj.name, "type:", obj.type)
            if obj.type == 'CURVE':
                export_curve(obj, f)
            elif obj.type == 'MESH':
                export_mesh(obj, f)
            elif include_grease_pencil and obj.type == 'GPENCIL':
                export_grease_pencil(obj, f)
            else:
                print("Skipping object type:", obj.type)
        f.write("END\n")
    print("Wrote", outpath)
    # optional: write compressed .mpzc (gzip)
    try:
        with open(outpath, "rb") as fin, gzip.open(compress_out, "wb") as fout:
            fout.writelines(fin)
        print("Wrote compressed", compress_out)
    except Exception as e:
        print("Could not write compressed file:", e)

# If run inside Blender Text Editor, you can call:
# export_selected_to_mpn()
# or call with custom paths:
# export_selected_to_mpn("/tmp/foo.mpn", "/tmp/foo.mpzc")
#
# Example: after running script, in Blender's Python Console:
# >>> import importlib
# >>> import export_selected_to_mpn
# >>> export_selected_to_mpn.export_selected_to_mpn()
#
# For convenience, run automatically if script executed:
if __name__ == "__main__":
    export_selected_to_mpn()
