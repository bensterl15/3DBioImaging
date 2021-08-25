function [faces_coordinates] = generate_faces_coordinates(faces, vertices, dim)
%GENERATE_FACES_COORDINATES Summary of this function goes here
%   Detailed explanation goes here
num_faces = size(faces, 1);
faces_coordinates = zeros(num_faces, dim);

for i = 1:num_faces
    current_face = faces(i, :);
    v1 = vertices(current_face(1), :); v2 = vertices(current_face(2), :);    v3 = vertices(current_face(3), :);
    faces_coordinates(i, :) = (v1 + v2 + v3) / 3;
end

end

