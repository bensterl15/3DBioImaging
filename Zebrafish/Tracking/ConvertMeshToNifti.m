xq = rand(1,100);                                           % Random X-Coordinates
yq = rand(1,100);                                           % Random Y-Coordinates
xv = [0.3, 0.3, 0.7, 0.7, 0.3];                             % Polygon X-Coordinates
yv = [0.3, 0.7, 0.7, 0.3, 0.3];                             % Polygon Y-Coordinates
[in,on] = inpolygon(xq,yq, xv,yv);                          % Logical Matrix
inon = in | on;                                             % Combine ‘in’ And ‘on’
idx = find(inon(:));                                        % Linear Indices Of ‘inon’ Points
xcoord = xq(idx);                                           % X-Coordinates Of ‘inon’ Points
ycoord = yq(idx);                                           % Y-Coordinates Of ‘inon’ Points
figure(1)
plot(xq, yq, 'bp')                                          % Plot All Points
hold on
plot(xv, yv, '-r')                                          % Plot Polygon
plot(xcoord, ycoord, 'gp')                                  % Overplot ‘inon’ Points
hold off

%%
load trackingData2.mat;

% Only consider absolute value of mean curvature:
curvature = abs(curvature);

pixel_density = 1e3;
% Select an epsilon for stereographic projection:
epsilon = 1;
curvature_threshold = 0;
turn_on_plots = true;%false;
write_cell_obj(faces, vertices, 'zebrafish.obj');

%% After processing in BFF/Python, continue process here
load trackingData2.mat
load zebrafish_flat.mat

% Cast to double:
zebrafish_flat_faces_3D = double(zebrafish_flat_faces_3D);

% Get the coordinates of the 3D faces that come from u-shape3D:
original_face_coordinates = generate_faces_coordinates(faces, vertices, 3);

% Get the coordinates of the 3D faces inside of BFF:
faces_coordinates = generate_faces_coordinates(zebrafish_flat_faces_3D, zebrafish_vertices, 3);

% Re-order curvature and segmentation to BFF's ordering:
[~, I1] = sortrows(original_face_coordinates);
[~, J1] = sort(I1);
[sorted, I2] = sortrows(faces_coordinates);
[~, J2] = sort(I2);
tmp = surfaceSegment(I1);
surfaceSegment = tmp(J2);
tmp = curvature(I1);
curvature = tmp(J2);
clear tmp;

% Ignore classes whose maximum curvature is below a threshold:
surfaceSegment = refine_segmentation(surfaceSegment, curvature, curvature_threshold);
%surfaceSegment(surfaceSegment ~= 22) = 0;

if turn_on_plots
    figure;
    scatter3(faces_coordinates(:,1), faces_coordinates(:,2), faces_coordinates(:,3), 25, surfaceSegment, 'filled');
    colorbar()
    title('Curvature');

    figure;
    scatter3(faces_coordinates(:,1), faces_coordinates(:,2), faces_coordinates(:,3), 25, curvature, 'filled');
    colorbar()
    title('Classes');
end

vertices = zebrafish_flat_vertices(:,1:2);
vertices = vertices / max(max(vertices));
faces_coordinates = generate_faces_coordinates(zebrafish_flat_faces, vertices, 2);

if turn_on_plots
    figure;
    subplot(211);
    scatter(faces_coordinates(:,1), faces_coordinates(:,2), 25, curvature, 'filled');
    colorbar()
    title('Curvature');

    subplot(212);
    scatter(faces_coordinates(:,1), faces_coordinates(:,2), 25, surfaceSegment, 'filled');
    colorbar();
    title('Protrusion Number');
end


% quantize the coordinates:
faces_coordinates = floor(pixel_density * faces_coordinates);
faces_coordinates = faces_coordinates - min(min(faces_coordinates)) + 1;
q_size = max(max(faces_coordinates));
quantized_curvature = zeros(pixel_density, pixel_density);
quantized_classes = zeros(pixel_density, pixel_density);

diam = pixel_density;
rad = diam / 2;

for i = 1:size(faces_coordinates, 1)
    x_ind = faces_coordinates(i,1);
    y_ind = faces_coordinates(i,2);
    if (x_ind - rad)^2 + (y_ind - rad)^2 < (rad)^2
        quantized_curvature(x_ind, y_ind) = pixel_density*curvature(i);
        quantized_classes(x_ind, y_ind) = surfaceSegment(i);
    end
end

output = imgaussfilt(quantized_curvature, 2);

output_classes = interpolate_2d_classes(quantized_classes, 22);

%%
if turn_on_plots
    [X,Y] = ndgrid(1:pixel_density, 1:pixel_density);
    figure;
    subplot(211);
    scatter(X(:), Y(:), 25, output(:), 'filled');
    %imshow(output / max(max(output)));
    %axis on;
    %xlim([400 500]);
    colorbar();
    title('Interpolated Curvature');

    subplot(212);
    %imshow(output_classes / max(max(output_classes)));
    %axis on;
    scatter(X(:), Y(:), 25, output_classes(:), 'filled');
    %xlim([400 500]);
    colorbar();
    title('Interpolated Protrusion Number');
end

'end'
%%
%save('output5.mat', 'output');
load output1.mat
output1 = output;
load output2.mat
output2 = output;
load output3.mat
output3 = output;
load output4.mat
output4 = output;
load output5.mat
output5 = output;

q_size = size(output1, 1);

[X,Y] = ndgrid(1:q_size, 1:q_size);

subplot(321);
scatter(X(:), Y(:), 25, output1(:), 'filled');
colorbar();

subplot(322);
scatter(X(:), Y(:), 25, output2(:), 'filled');
colorbar();

subplot(323);
scatter(X(:), Y(:), 25, output3(:), 'filled');
colorbar();

subplot(324);
scatter(X(:), Y(:), 25, output4(:), 'filled');
colorbar();

subplot(325);
scatter(X(:), Y(:), 25, output5(:), 'filled');
colorbar();


%%
pixel_density = 1e2;

x_loc = pixel_density * [0.1 0.3 0.5 0.7];
y_loc = pixel_density * [0.7 0.1 0.3 0.2];
vals = [1 2 3 4];

interpolated = zeros(pixel_density, pixel_density);
for x_prime = 1:pixel_density
    for y_prime = 1:pixel_density
        for i = 1:length(x_loc)
           interpolated(x_prime, y_prime) = interpolated(x_prime, y_prime) ...
            + sinc(sqrt((x_prime - x_loc(i))^2 + (y_prime - y_loc(i))^2)) * vals(i);
        end
    end
end

subplot(211);
original = zeros(pixel_density, pixel_density);
original(10, 70) = 1;
original(30, 10) = 2;
original(50, 30) = 3;
original(70, 20) = 4;
imshow(original)

subplot(212);
interpolated = interpolated - min(min(interpolated));
interpolated = interpolated / max(max(interpolated));
imshow(interpolated);