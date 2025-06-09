use bevy::prelude::*;
use bevy::ecs::schedule::States;
use bevy::input::keyboard::KeyCode;
use bevy::input::ButtonInput;
use rand::prelude::*;

#[derive(States, Debug, Clone, Eq, PartialEq, Hash, Default)]
enum GameState {
    #[default]
    Menu,
    Jogando,
    Fim,
}

#[derive(Component)]
struct Jogador;

#[derive(Component)]
struct Item;

#[derive(Component)]
struct Inimigo;

#[derive(Component)]
struct TextoUI;

#[derive(Resource)]
struct Pontuacao(u32);

#[derive(Resource)]
struct TempoRestante(Timer);

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .insert_resource(Pontuacao(0))
        .insert_resource(TempoRestante(Timer::from_seconds(15.0, TimerMode::Once)))
        .insert_state(GameState::Menu)
        .add_systems(Startup, setup_camera)
        .add_systems(Update, menu_input.run_if(in_state(GameState::Menu)))
        .add_systems(OnEnter(GameState::Jogando), setup_jogo)
        .add_systems(Update, (
            movimento_jogador,
            mover_inimigos,
            coletar_itens,
            verificar_colisoes,
            atualizar_timer,
        ).run_if(in_state(GameState::Jogando)))
        .add_systems(OnEnter(GameState::Fim), mostrar_resultado)
        .add_systems(Update, voltar_ao_menu.run_if(in_state(GameState::Fim)))
        .run();
}

fn setup_camera(mut commands: Commands) {
    commands.spawn(Camera2dBundle::default());
}

fn menu_input(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    keys: Res<ButtonInput<KeyCode>>,
    mut next_state: ResMut<NextState<GameState>>,
    existing_text: Query<Entity, With<TextoUI>>,
) {
    if keys.just_pressed(KeyCode::Space) {
        next_state.set(GameState::Jogando);
        for entity in existing_text.iter() {
            commands.entity(entity).despawn();
        }
    } else if existing_text.is_empty() {
        commands.spawn((
            TextBundle::from_section(
                "Pressione Espaço para Começar",
                TextStyle {
                    font: asset_server.load("fonts/FiraSans-Bold.ttf"),
                    font_size: 40.0,
                    color: Color::WHITE,
                },
            )
            .with_style(Style {
                position_type: PositionType::Absolute,
                top: Val::Percent(40.0),
                left: Val::Percent(20.0),
                ..default()
            }),
            TextoUI,
        ));
    }
}

fn voltar_ao_menu(
    keys: Res<ButtonInput<KeyCode>>,
    mut next_state: ResMut<NextState<GameState>>,
    mut commands: Commands,
    entities: Query<Entity, (Without<Camera2d>, Without<Camera>)>,
) {
    if keys.just_pressed(KeyCode::Space) {
        for entity in entities.iter() {
            commands.entity(entity).despawn_recursive();
        }
        next_state.set(GameState::Menu);
    }
}

fn setup_jogo(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut score: ResMut<Pontuacao>,
    mut timer: ResMut<TempoRestante>,
) {
    score.0 = 0;
    timer.0.reset();

    commands.spawn((
        TextBundle::from_section(
            "Pontuação: 0\nTempo: 30",
            TextStyle {
                font: asset_server.load("fonts/FiraSans-Bold.ttf"),
                font_size: 30.0,
                color: Color::WHITE,
            },
        )
        .with_style(Style {
            position_type: PositionType::Absolute,
            top: Val::Px(10.0),
            left: Val::Px(10.0),
            ..default()
        }),
        TextoUI,
    ));

    commands.spawn((
        SpriteBundle {
            sprite: Sprite {
                color: Color::BLUE,
                custom_size: Some(Vec2::splat(40.0)),
                ..default()
            },
            transform: Transform::from_xyz(0.0, 0.0, 0.0),
            ..default()
        },
        Jogador,
    ));

    let mut rng = thread_rng();
    for _ in 0..10 {
        let x = rng.gen_range(-300.0..300.0);
        let y = rng.gen_range(-200.0..200.0);
        commands.spawn((
            SpriteBundle {
                sprite: Sprite {
                    color: Color::GREEN,
                    custom_size: Some(Vec2::splat(25.0)),
                    ..default()
                },
                transform: Transform::from_xyz(x, y, 0.0),
                ..default()
            },
            Item,
        ));
    }

    let cantos = [
        (-300.0, -200.0),
        (300.0, -200.0),
        (300.0, 200.0),
        (-300.0, 200.0),
    ];
    for (x, y) in cantos.iter().take(3) {
        commands.spawn((
            SpriteBundle {
                sprite: Sprite {
                    color: Color::RED,
                    custom_size: Some(Vec2::splat(30.0)),
                    ..default()
                },
                transform: Transform::from_xyz(*x, *y, 0.0),
                ..default()
            },
            Inimigo,
        ));
    }
}

fn movimento_jogador(
    keys: Res<ButtonInput<KeyCode>>,
    mut query: Query<&mut Transform, With<Jogador>>,
    time: Res<Time>,
) {
    let mut transform = query.single_mut();
    let mut direcao = Vec3::ZERO;

    if keys.pressed(KeyCode::KeyW) {
        direcao.y += 1.0;
    }
    if keys.pressed(KeyCode::KeyS) {
        direcao.y -= 1.0;
    }
    if keys.pressed(KeyCode::KeyA) {
        direcao.x -= 1.0;
    }
    if keys.pressed(KeyCode::KeyD) {
        direcao.x += 1.0;
    }

    transform.translation += direcao.normalize_or_zero() * 200.0 * time.delta_seconds();
}

fn mover_inimigos(
    mut inimigos: Query<&mut Transform, With<Inimigo>>,
    jogador: Query<&Transform, (With<Jogador>, Without<Inimigo>)>,
    time: Res<Time>,
) {
    let alvo = jogador.single().translation;
    for mut transform in inimigos.iter_mut() {
        let dir = (alvo - transform.translation).normalize_or_zero();
        transform.translation += dir * 100.0 * time.delta_seconds();
    }
}

fn coletar_itens(
    mut commands: Commands,
    mut score: ResMut<Pontuacao>,
    jogador: Query<&Transform, With<Jogador>>,
    mut items: Query<(Entity, &Transform), With<Item>>,
    mut textos: Query<&mut Text, With<TextoUI>>,
    tempo: Res<TempoRestante>,
) {
    let jogador_pos = jogador.single().translation;
    for (ent, t_item) in items.iter_mut() {
        if jogador_pos.distance(t_item.translation) < 35.0 {
            commands.entity(ent).despawn();
            score.0 += 1;

            let mut texto = textos.single_mut();
            texto.sections[0].value =
                format!("Pontuação: {}\nTempo: {:.0}", score.0, tempo.0.remaining_secs());
        }
    }
}

fn verificar_colisoes(
    jogador: Query<&Transform, With<Jogador>>,
    inimigos: Query<&Transform, With<Inimigo>>,
    items: Query<&Item>,
    mut next_state: ResMut<NextState<GameState>>,
    score: Res<Pontuacao>,
    tempo: Res<TempoRestante>,
) {
    let pos_j = jogador.single().translation;
    for t_inimigo in inimigos.iter() {
        if pos_j.distance(t_inimigo.translation) < 35.0 {
            next_state.set(GameState::Fim);
            return;
        }
    }

    if tempo.0.finished() || score.0 >= 10 || items.iter().count() == 0 {
        next_state.set(GameState::Fim);
    }
}

fn atualizar_timer(
    mut timer: ResMut<TempoRestante>,
    time: Res<Time>,
    mut textos: Query<&mut Text, With<TextoUI>>,
    score: Res<Pontuacao>,
) {
    timer.0.tick(time.delta());
    let mut texto = textos.single_mut();
    texto.sections[0].value =
        format!("Pontuação: {}\nTempo: {:.0}", score.0, timer.0.remaining_secs());
}

fn mostrar_resultado(
    score: Res<Pontuacao>,
    tempo: Res<TempoRestante>,
    mut commands: Commands,
    asset_server: Res<AssetServer>,
) {
    let mensagem = if score.0 >= 10 && tempo.0.remaining_secs() > 0.0 {
        "VOCÊ VENCEU!"
    } else {
        "VOCÊ PERDEU!"
    };

    commands.spawn((
        TextBundle::from_section(
            format!(
                "{}\nPontuação final: {}\nPressione Espaço para sair.",
                mensagem, score.0
            ),
            TextStyle {
                font: asset_server.load("fonts/FiraSans-Bold.ttf"),
                font_size: 40.0,
                color: Color::YELLOW,
            },
        )
        .with_style(Style {
            position_type: PositionType::Absolute,
            top: Val::Percent(40.0),
            left: Val::Percent(20.0),
            ..default()
        }),
        TextoUI,
    )); 
}
