function [OBJ] = write_cell_obj(faces, vertices, file_name)
%WRITE_CELL_OBJ Summary of this function goes here
%   Detailed explanation goes here
OBJ.vertices = vertices;
%   OBJ.vertices_normal = N;
%   OBJ.material = material;
OBJ.objects(1).type='g';
OBJ.objects(1).data='skin';
OBJ.objects(2).type='usemtl';
OBJ.objects(2).data='skin';
OBJ.objects(3).type='f';
OBJ.objects(3).data.vertices = faces;
OBJ.objects(3).data.normal = faces;
write_wobj(OBJ, file_name);

end

