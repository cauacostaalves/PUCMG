//! Jogo Bevy com menu, jogabilidade, timer, inimigos e sistema de vitória/derrota comentado linha por linha.

use bevy::prelude::*;
use bevy::ecs::schedule::States;
use bevy::input::keyboard::KeyCode;
use bevy::input::ButtonInput;
use rand::prelude::*;

// Define os diferentes estados do jogo: Menu inicial, Jogo rodando e Tela de Fim
#[derive(States, Debug, Clone, Eq, PartialEq, Hash, Default)]
enum GameState {
    #[default]
    Menu,
    Jogando,
    Fim,
}

// Marca o jogador
#[derive(Component)]
struct Jogador;

// Marca os itens coletáveis
#[derive(Component)]
struct Item;

// Marca os inimigos
#[derive(Component)]
struct Inimigo;

// Marca os textos exibidos na UI
#[derive(Component)]
struct TextoUI;

// Armazena a pontuação do jogador
#[derive(Resource)]
struct Pontuacao(u32);

// Armazena o tempo restante do jogo
#[derive(Resource)]
struct TempoRestante(Timer);

fn main() {
    App::new()
        .add_plugins(DefaultPlugins) // Plugins padrão do Bevy (janela, render, input, etc)
        .insert_resource(Pontuacao(0)) // Inicia pontuação com 0
        .insert_resource(TempoRestante(Timer::from_seconds(10.0, TimerMode::Once))) // Timer de 10 segundos (ajustável)
        .insert_state(GameState::Menu) // Começa no estado de Menu
        .add_systems(Startup, setup_camera) // Inicializa a câmera
        .add_systems(Update, menu_input.run_if(in_state(GameState::Menu))) // Entrada no menu
        .add_systems(OnEnter(GameState::Jogando), setup_jogo) // Inicia o jogo
        .add_systems(Update, ( // Sistemas que rodam enquanto o jogo está ativo
            movimento_jogador, // Permite mover o jogador com WASD
            mover_inimigos,    // Inimigos perseguem o jogador
            coletar_itens,     // Jogador coleta itens verdes
            verificar_colisoes,// Verifica vitória/derrota
            atualizar_timer,   // Atualiza tempo e texto
        ).run_if(in_state(GameState::Jogando)))
        .add_systems(OnEnter(GameState::Fim), mostrar_resultado) // Mostra tela final
        .add_systems(Update, voltar_ao_menu.run_if(in_state(GameState::Fim))) // Retorna ao menu após fim
        .run();
}

// Cria a câmera 2D
fn setup_camera(mut commands: Commands) {
    commands.spawn(Camera2dBundle::default());
}

// Entrada no menu inicial
fn menu_input(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    keys: Res<ButtonInput<KeyCode>>,
    mut next_state: ResMut<NextState<GameState>>,
    existing_text: Query<Entity, With<TextoUI>>,
) {
    if keys.just_pressed(KeyCode::Space) {
        // Transita para o estado de Jogo ao apertar espaço
        next_state.set(GameState::Jogando);
        // Remove texto do menu
        for entity in existing_text.iter() {
            commands.entity(entity).despawn();
        }
    } else if existing_text.is_empty() {
        // Se não tem texto na tela, mostra instrução
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

// Após o jogo terminar, espera espaço para voltar ao menu
fn voltar_ao_menu(
    keys: Res<ButtonInput<KeyCode>>,
    mut next_state: ResMut<NextState<GameState>>,
    mut commands: Commands,
    entities: Query<Entity, (Without<Camera2d>, Without<Camera>)>,
) {
    if keys.just_pressed(KeyCode::Space) {
        // Remove todos os elementos visuais exceto câmera
        for entity in entities.iter() {
            commands.entity(entity).despawn_recursive();
        }
        // Retorna ao estado de Menu
        next_state.set(GameState::Menu);
    }
}

// Configura a fase: jogador, itens, inimigos e HUD
fn setup_jogo(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut score: ResMut<Pontuacao>,
    mut timer: ResMut<TempoRestante>,
) {
    score.0 = 0; // Reseta pontuação
    timer.0.reset(); // Reseta tempo

    // HUD com pontuação e tempo
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
            top: Val::Px(10.0), // Posição do texto no topo da tela
            left: Val::Px(10.0), // Alinhado à esquerda
            ..default()
        }),
        TextoUI,
    ));

    // Cria jogador no centro da tela
    commands.spawn((
        SpriteBundle {
            sprite: Sprite {
                color: Color::BLUE, // Jogador azul
                custom_size: Some(Vec2::splat(40.0)), // Tamanho do sprite
                ..default()
            },
            transform: Transform::from_xyz(0.0, 0.0, 0.0), // Posição inicial no centro
            ..default()
        },
        Jogador,
    ));

    // Gera 10 itens em posições aleatórias no mapa
    let mut rng = thread_rng();
    for _ in 0..10 {
        let x = rng.gen_range(-300.0..300.0); // Posição horizontal aleatória
        let y = rng.gen_range(-200.0..200.0); // Posição vertical aleatória
        commands.spawn((
            SpriteBundle {
                sprite: Sprite {
                    color: Color::GREEN, // Itens são verdes
                    custom_size: Some(Vec2::splat(25.0)), // Tamanho menor que o jogador
                    ..default()
                },
                transform: Transform::from_xyz(x, y, 0.0), // Posição aleatória na tela
                ..default()
            },
            Item,
        ));
    }

    // Posiciona 3 inimigos vermelhos nos cantos do mapa
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
                    color: Color::RED, // Inimigos são vermelhos
                    custom_size: Some(Vec2::splat(30.0)),
                    ..default()
                },
                transform: Transform::from_xyz(*x, *y, 0.0), // Canto da tela
                ..default()
            },
            Inimigo,
        ));
    }
}
