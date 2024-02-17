from launch import LaunchDescription
import launch_ros.actions

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            namespace= "homework", package='homework', executable='homework_node', output='screen'),
        launch_ros.actions.Node(
            namespace= "answer", package='answer', executable='answer_node', output='screen'),
    ])
