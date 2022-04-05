#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/String.h"
#include <map>
#include <string>
bool rasa=false;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
#include <string>
bool rasa=false;
using namespace std;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
map<string, int> mainlocation;
int loc1=1;
int loc2=2;
int loc3=3;
int loc4=4;
float x[4][3] = {{1,-6.5,1}, {2,-6.5,1}, {3,-6.5,1},{0,0,1}};
float rasapos[3]={0,0,0};
        // insert elements in random order
mainlocation.insert(pair<string, int>("toilet", loc1));
mainlocation.insert(pair<string, int>("room1", loc2));
mainlocation.insert(pair<string, int>("room2", loc3));
mainlocation.insert(pair<string, int>("receptionist", loc4));
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    map<int, int>::iterator itr;
    for (itr = mainlocation.begin(); itr != mainlocation.end(); ++itr) {
        if(itr->first==(msg->data.c_str())){
            cout<<"\t"<<itr->first<<" is at position: ";
            cout <<x[(itr->second)-1][0]<<'\t' <<x[itr->second-1][1]<<'\t'<<x[itr->second-1][2]<<endl;
            rasapos[0]=x[(itr->second)-1][0];
            rasapos[1]=x[(itr->second)-1][1];
            rasapos[2]=x[(itr->second)-1][2];
            rasa=true;
        }
    }

  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char** argv){

  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("rasachatbot", 1000, chatterCallback);
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  while (1){
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 1.0;
  goal.target_pose.pose.position.y = -6.3;
  goal.target_pose.pose.orientation.w = 1.0;


  if (rasa){
    ac.sendGoal(goal);
    ROS_INFO("Sending goal");
    rasa=false;
  }
  //ac.waitForResult();
  actionlib::SimpleClientGoalState state = ac.getState();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Action finished: %s",state.toString().c_str());
    ROS_INFO("Hooray, the base moved 1 meter forward");
  }
  else if (ac.getState() == actionlib::SimpleClientGoalState::LOST){
    ROS_INFO("Action failed: LOST");
    ROS_INFO("no goal recieved");
  }
  ros::spinOnce();
  }
  return 0;
}
