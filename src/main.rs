use bevy::{
    prelude::*,
    pbr::MeshMaterial3d,
    input::mouse::{AccumulatedMouseMotion, AccumulatedMouseScroll},
};

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_systems(Startup, setup)
        .add_systems(Update, (mouse_click_system, mouse_move_system))
        .run();
}

fn setup(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    commands.spawn((
        Mesh3d(meshes.add(Sphere::default())),
        MeshMaterial3d(materials.add(Color::WHITE)),
    ));

    commands.spawn((
        Camera3d::default(),
        Transform::from_xyz(0.0, 1.0, 2.0).looking_at(Vec3::ZERO, Vec3::Y),
    ));

    commands.spawn((
        DirectionalLight::default(),
        Transform::from_xyz(3.0, 3.0, 3.0).looking_at(Vec3::ZERO, Vec3::Y),
    ));
}

fn mouse_click_system(mouse_button_input: Res<ButtonInput<MouseButton>>) {
    if mouse_button_input.pressed(MouseButton::Left) {
        info!("left mouse currently pressed");
    }

    if mouse_button_input.just_pressed(MouseButton::Left) {
        info!("left mouse just pressed");
    }

    if mouse_button_input.just_released(MouseButton::Left) {
        info!("left mouse just released");
    }
}

fn mouse_move_system(
    accumulated_mouse_motion: Res<AccumulatedMouseMotion>,
    accumulated_mouse_scroll: Res<AccumulatedMouseScroll>,
) {
    if accumulated_mouse_motion.delta != Vec2::ZERO {
        let delta = accumulated_mouse_motion.delta;
        info!("mouse moved ({}, {})", delta.x, delta.y);
    }
    if accumulated_mouse_scroll.delta != Vec2::ZERO {
        let delta = accumulated_mouse_scroll.delta;
        info!("mouse scrolled ({}, {})", delta.x, delta.y);
    }
}
