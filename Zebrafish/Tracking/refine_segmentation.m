function [surfaceSegment] = refine_segmentation(surfaceSegment, curvature, curvature_threshold)
% FIRST, eliminate classes that do not contain high enough curvatures:
class_levels = unique(surfaceSegment);
for level_index = 1:length(class_levels)
    level = class_levels(level_index);
    max_curv_in_class = max(curvature(surfaceSegment == level));

    % If the max curvature is not high enough, ignore this class:
    if(max_curv_in_class < curvature_threshold)
        surfaceSegment(surfaceSegment == level) = 0;
    end
end

% Simplify the class labels

class_levels = unique(surfaceSegment);
for level_index = 1:length(class_levels)
    level = class_levels(level_index);
    surfaceSegment(surfaceSegment == level) = level_index - 1;
end

end

