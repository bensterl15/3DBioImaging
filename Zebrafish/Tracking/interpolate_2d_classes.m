function [output_classes] = interpolate_2d_classes(quantized_classes, class)
%INTERPOLATE_2D_CLASSES Summary of this function goes here
%   Detailed explanation goes here

im_width = size(quantized_classes, 1);
classes = quantized_classes;

classes(quantized_classes ~= class) = 0;
[row, col] = ind2sub(size(classes),find(classes));
X = [row, col];
[k, ~] = convhull(X);

polygon_points = zeros(size(k, 1),2);

for w = 1:size(k,1)
        i = X(k(w), 1);
        j = X(k(w), 2);
        polygon_points(w, :) = [i, j];
        classes(i, j) = 1;
end

X = zeros(im_width * im_width, 2);

k = 1;
for i = 1:im_width
    for j = 1:im_width
        X(k, :) = [j, i];
        k = k + 1;
    end
end

[in,~] = inpolygon(X(:,1), X(:,2), polygon_points(:,1), polygon_points(:,2));
classes(in) = class;

output_classes = quantized_classes;
output_classes(output_classes == class) = 0;
output_classes(in) = class;

%{
subplot(211);
imshow(quantized_classes / max(max(quantized_classes)) );
ylim([320 420]);
xlim([820 920]);

subplot(212);
imshow(output_classes);
ylim([320 420]);
xlim([820 920]);
%}

end

