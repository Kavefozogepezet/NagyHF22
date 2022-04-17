
from io import FileIO
from tokenize import Double
import bpy
from bpy_extras.io_utils import ExportHelper

bl_info = {
    "name": "SHPS Export",
    "descibtion": "Export objects for use in CollSys",
    "author": "Apostagi Bálint",
    "version": (0, 1, 0),
    "blender": (2, 80, 0),
    "location": "File > Export",
    "warning": "beta",
    "support": "COMMUNITY",
    "category": "Import-Export"
}

class BezierSegment:
    def __init__(self, p0, p1, p2, p3):
        self.vecs:list = []
        self.vecs.extend([
            p3 + 3.0 * p1 - 3.0 * p2 - p0,
            3.0 * p2 - 6.0 * p1 + 3.0 * p0,
            3.0 * p1 - 3.0 * p0,
            p0
        ])

class ExportSHPS(bpy.types.Operator, ExportHelper):
    """Export objects to shps file"""
    bl_idname = "export.shps"
    bl_label = "shps"
    bl_options = {"REGISTER"}

    filename_ext = ".shps"

    @staticmethod
    def __curve(file:FileIO, obj):
        points:list = []
        print(len(obj.data.splines[0].bezier_points), end=' ', file=file)
        for point in obj.data.splines[0].bezier_points:
            points.extend([point.handle_left, point.co, point.handle_right])
        points.extend([points[0], points[1]])
        del points[0]
        for idx in range(0, len(points)-2, 3):
            seg = BezierSegment(points[idx], points[idx+1], points[idx+2], points[idx+3])
            for coord in seg.vecs:
                print(coord.x, coord.y, end=' ', file=file)
        print(end='\n', file=file)

    @staticmethod
    def __mesh(file:FileIO, obj):
        print(len(obj.data.vertices), end=' ', file=file)
        for vert in obj.data.vertices:
            coord = vert.co
            print(coord.x, coord.y, end=' ', file=file)
        print(end='\n', file=file)

    export_funcs = {
        "CURVE": ("bezier", __curve),
        "MESH": ("polygon", __mesh)
    }

    @staticmethod
    def __transform(file:FileIO, obj):
        pos = obj.location
        rot = obj.rotation_euler
        scale = obj.scale
        print(pos.x, pos.y, rot.z, scale.x, scale.y, end=' ', file=file)

    def execute(self, context):
        scene = bpy.context.scene
        file = open(self.filepath, "w")

        for obj in scene.objects:
            if obj.type in self.export_funcs:
                val = self.export_funcs[obj.type]
                print("new", val[0], end=' ', file=file)
                self.__transform(file, obj)
                print(obj.name, end=' ', file=file)
                val[1](file, obj)
            else:
                print("Object type", obj.type, "is not supported.")
        
        file.close()
        return {"FINISHED"}

def menu_func(self, context):
    self.layout.operator(ExportSHPS.bl_idname)

def register():
    bpy.utils.register_class(ExportSHPS)
    bpy.types.TOPBAR_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_class(ExportSHPS)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
