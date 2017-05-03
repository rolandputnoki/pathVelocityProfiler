//
// Created by osboxes on 23/03/17.
//

#include <ros/ros.h>
#include "std_msgs/String.h"
#include "path_sampler/SampledPath.h"
#include <tf/transform_broadcaster.h>
#include "VelocityProfileBuilder.h"
#include "path_velocity_profiler/VelProfPoint.h"
#include "path_velocity_profiler/VelProf.h"
#include "path_velocity_profiler/PathVelProfile.h"


using namespace path_sampler;
using namespace path_velocity_profiler;


geometry_msgs::Quaternion toQuaternion(double y_rot, double x_rot, double z_rot)
{
    geometry_msgs::Quaternion q;
    double t0 = std::cos(z_rot * 0.5);
    double t1 = std::sin(z_rot * 0.5);
    double t2 = std::cos(x_rot * 0.5);
    double t3 = std::sin(x_rot * 0.5);
    double t4 = std::cos(y_rot * 0.5);
    double t5 = std::sin(y_rot * 0.5);

    q.w = t0 * t2 * t4 + t1 * t3 * t5;
    q.x = t0 * t3 * t4 - t1 * t2 * t5;
    q.y = t0 * t2 * t5 + t1 * t3 * t4;
    q.z = t1 * t2 * t4 - t0 * t3 * t5;
    return q;
}


void velocity_profiler_Callback(SampledPath sp){
    int num_of_sections = sp.sections.size();
    PathVelProfile pv;
    /*
    for(int i = 0; i < num_of_sections; i++){
        VelocityProfileBuilder v(sp.sections[i]);
        v.maxVelProfile.writePointsToFile();
        ROS_INFO("Hello!");
    }
     */

    /*
    std::string s("test_path.txt");
    VelocityProfileBuilder vpb(s);
    vpb.maxVelProfile.writePointsToFile(1000);
    vpb.build();

     */

    for(size_t i = 0; i< num_of_sections; i++){
        VelocityProfileBuilder vpb(sp.sections[i]);
        vpb.maxVelProfile.writePointsToFile(1000 + i);
        vpb.build();
        VelProf vp;
        size_t length_of_profile = vpb.finalProfile.getLength();
        vpb.finalProfile.writePointsToFile(2000 + i);
        for(size_t j = 0; j<length_of_profile; j++){
            VelProfPoint vpp;
            vpp.distance = vpb.finalProfile.getPosAtIndex(j);
            vpp.velocity = vpb.finalProfile.getVelAt(j);
            vpp.time = vpb.finalProfile.getTimeAt(j);
            vp.velProfile.push_back(vpp);
        }

        pv.pathProfile.push_back(vp);
    }


    ROS_INFO("Hello!");

    /*
    ros::NodeHandle n2;
    ros::Publisher curv_pub = n2.advertise<geometry_msgs::Pose>("Curvature",1000);
    Sampled_path sp;
    geometry_msgs::PoseArray p_a;
    geometry_msgs::Pose pose1;
    while(!spa.sections.empty()){
        sp = spa.sections.back();
        spa.sections.erase(spa.sections.end());
        while(!sp.sample_points.empty()){
            geometry_msgs::Pose pose;
            pose.position.y = sp.sample_points.back().curvature*500;
            pose.position.x = sp.sample_points.back().position/100;
            sp.sample_points.erase(sp.sample_points.end());
            ROS_INFO("Gorbulet: %lf", pose.position.y);
            p_a.poses.push_back(pose);
//            curv_pub.publish(pose);
        }
    }
    ros::Publisher pose_pub = n2.advertise<geometry_msgs::PoseArray>("pose_array", 100);
    p_a.header.frame_id = "/my_frame";
    p_a.header.stamp = ros::Time::now();
    pose_pub.publish(p_a);
    ros::spin();

     */
}

int main(int argc, char **argv){
    ros::init(argc, argv, "velocity_profiler");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("Sampled_path", 1000, velocity_profiler_Callback);
    ros::spin();
    return 0;
}