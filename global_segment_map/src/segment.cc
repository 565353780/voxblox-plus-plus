#include "global_segment_map/segment.h"
namespace voxblox {

Segment::Segment(const pcl::PointCloud<voxblox::PointType>& point_cloud,
                 const Transformation& T_G_C)
    : T_G_C_(T_G_C), semantic_label_(0u), instance_label_(0u) {
  points_C_.reserve(point_cloud.points.size());
  colors_.reserve(point_cloud.points.size());

  for (size_t i = 0; i < point_cloud.points.size(); ++i) {
    if (!std::isfinite(point_cloud.points[i].x) ||
        !std::isfinite(point_cloud.points[i].y) ||
        !std::isfinite(point_cloud.points[i].z)) {
      continue;
    }

    points_C_.push_back(Point(point_cloud.points[i].x, point_cloud.points[i].y,
                              point_cloud.points[i].z));

    colors_.push_back(Color(point_cloud.points[i].r, point_cloud.points[i].g,
                            point_cloud.points[i].b, point_cloud.points[i].a));
  }
}

Segment::Segment(const pcl::PointCloud<voxblox::PointLabelType>& point_cloud,
                 const Transformation& T_G_C)
    : T_G_C_(T_G_C), label_(point_cloud.points[0].label) {
  points_C_.reserve(point_cloud.points.size());
  colors_.reserve(point_cloud.points.size());

  for (size_t i = 0u; i < point_cloud.points.size(); ++i) {
    if (!std::isfinite(point_cloud.points[i].x) ||
        !std::isfinite(point_cloud.points[i].y) ||
        !std::isfinite(point_cloud.points[i].z)) {
      continue;
    }

    points_C_.push_back(Point(point_cloud.points[i].x, point_cloud.points[i].y,
                              point_cloud.points[i].z));

    colors_.push_back(Color(point_cloud.points[i].r, point_cloud.points[i].g,
                            point_cloud.points[i].b, point_cloud.points[i].a));
  }
}

Segment::Segment(
    const pcl::PointCloud<voxblox::PointSemanticInstanceType>& point_cloud,
    const Transformation& T_G_C)
    : T_G_C_(T_G_C),
      semantic_label_(point_cloud.points[0].semantic_label),
      instance_label_(point_cloud.points[0].instance_label) {

  std::vector<std::pair<int, int>> semantic_label_vec;
  std::vector<std::pair<int, int>> instance_label_vec;
  for(auto point : point_cloud.points)
  {
    bool have_semantic_label = false;
    bool have_instance_label = false;

    for(auto& label_pair : semantic_label_vec)
    {
      if(label_pair.first == point.semantic_label)
      {
        ++label_pair.second;
        have_semantic_label = true;
        break;
      }
    }
    if(!have_semantic_label)
    {
      std::pair<int, int> current_label_pair;
      current_label_pair.first = point.semantic_label;
      current_label_pair.second = 1;
      semantic_label_vec.emplace_back(current_label_pair);
    }

    for(auto& label_pair : instance_label_vec)
    {
      if(label_pair.first == point.instance_label)
      {
        ++label_pair.second;
        have_instance_label = true;
        break;
      }
    }
    if(!have_instance_label)
    {
      std::pair<int, int> current_label_pair;
      current_label_pair.first = point.instance_label;
      current_label_pair.second = 1;
      instance_label_vec.emplace_back(current_label_pair);
    }
  }

  if(semantic_label_ != 0 && instance_label_ != 0)
  {
    LOG(WARNING) << "pointcloud.points.size() = " <<
      point_cloud.points.size();
    LOG(WARNING) << "the input semantic and instance label list is :";
    for(auto label_pair : semantic_label_vec)
    {
      LOG(WARNING) << "semantic : " << label_pair.first
        << " , " << "num : " << label_pair.second;
    }
    for(auto label_pair : instance_label_vec)
    {
      LOG(WARNING) << "instance : " << label_pair.first
        << " , " << "num : " << label_pair.second;
    }
    LOG(WARNING) << "================";
  }

  points_C_.reserve(point_cloud.points.size());
  colors_.reserve(point_cloud.points.size());

  for (size_t i = 0u; i < point_cloud.points.size(); ++i) {
    if (!std::isfinite(point_cloud.points[i].x) ||
        !std::isfinite(point_cloud.points[i].y) ||
        !std::isfinite(point_cloud.points[i].z)) {
      continue;
    }

    points_C_.push_back(Point(point_cloud.points[i].x, point_cloud.points[i].y,
                              point_cloud.points[i].z));

    colors_.push_back(Color(point_cloud.points[i].r, point_cloud.points[i].g,
                            point_cloud.points[i].b, point_cloud.points[i].a));
  }
}

}  // namespace voxblox
