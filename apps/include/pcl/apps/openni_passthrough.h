/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2009-2011, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

#include <pcl/apps/openni_passthrough_qt.h>
#include <pcl/apps/timer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace Ui {
class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OpenNIPassthrough : public QMainWindow {
  Q_OBJECT
public:
  using Cloud = pcl::PointCloud<pcl::PointXYZRGBA>;
  using CloudPtr = Cloud::Ptr;
  using CloudConstPtr = Cloud::ConstPtr;

  OpenNIPassthrough(pcl::OpenNIGrabber& grabber);

  ~OpenNIPassthrough()
  {
    if (grabber_.isRunning())
      grabber_.stop();
  }

  void
  cloud_cb(const CloudConstPtr& cloud);

protected:
  void
  refreshView();

  pcl::visualization::PCLVisualizer::Ptr vis_;
  pcl::OpenNIGrabber& grabber_;
  std::string device_id_;
  CloudPtr cloud_pass_;
  pcl::PassThrough<pcl::PointXYZRGBA> pass_;

private:
  QMutex mtx_;
  Ui::MainWindow* ui_;
  QTimer* vis_timer_;

public Q_SLOTS:
  void
  adjustPassThroughValues(int new_value)
  {
    pass_.setFilterLimits(0.0f, float(new_value) / 10.0f);
    PCL_INFO("Changed passthrough maximum value to: %f\n", float(new_value) / 10.0f);
  }

private Q_SLOTS:
  void
  timeoutSlot();

Q_SIGNALS:
  void
  valueChanged(int new_value);
};
