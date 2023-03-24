use axum::{
    routing::{get},
    Json, Router, extract::Query,
};
use db::DataEntry;
use serde::{Serialize};
use std::{net::SocketAddr, collections::HashMap, time::{SystemTime}};
use rand::Rng;

mod db;

fn generate_data(n: u32) -> Vec<db::DataEntry> {
    // declare new DataEntry vector of size N
    let size = n as usize;
    let mut data = Vec::<db::DataEntry>::new();

    // get random number between 0 and 100000

    for i in 0..size {
        let rand: u32 = rand::thread_rng().gen_range(0..100000);
        data.push(db::DataEntry {
            name: format!("name_{}", (10000000 - i)),
            value: format!("value_{}{}", rand, i),
        })
    }

    data
}

#[derive(strum_macros::EnumString, strum_macros::Display, Serialize)]
enum Status {
    #[strum(serialize = "success")]
    SUCCESS,
    #[strum(serialize = "error")]
    ERROR,
}

#[derive(Serialize)]
struct Response {
    pub status: Status,
    pub message: String,
    #[serde(skip_serializing_if  = "Option::is_none")]
    pub data: Option<Vec<db::DataEntry>>
}

impl Response {
    pub fn new_error<S: Into<String>>(s: S) -> Response {
        Response {
            status: Status::ERROR,
            message: s.into(),
            data: None
        }
    }
}

#[derive(Clone)]
struct AppState {
    pub db: db::Database,
}

#[tokio::main]
async fn main() {
    let state = AppState{
        db: db::Database::new()
    };

    let app = Router::new()
        .route("/", get(root))
        .route("/reset_and_insert_data", get(reset_and_insert_data))
        .route("/get_first_values", get(get_first_values))
        .with_state(state);

    let addr = SocketAddr::from(([127, 0, 0, 1], 3003));
    println!("Listening on http://{}", addr);

    axum::Server::bind(&addr)
        .serve(app.into_make_service())
        .await
        .unwrap();
}

async fn root() -> &'static str{
    "Hello, World!"
}

async fn reset_and_insert_data(
    state: axum::extract::State<AppState>,
    params: Query<HashMap<String, String>>,
) -> Json<Response> {
    state.db.reset_db().await;

    let param_n: u32 = match params.get(&"n".to_string()).and_then(|n| n.parse::<u32>().ok()) {
        None => return Json(Response::new_error("Missing or invalid parameter n.")),
        Some(n) => n
    };

    let start_time = SystemTime::now();

    for DataEntry{name, value} in generate_data(param_n) {
        state.db.insert_entry(&name, &value).await
    }

    let elapsed = start_time.elapsed().unwrap().as_millis() as u32;

    return Json(Response{
        data: None,
        status: Status::SUCCESS,
        message: format!("Inserted {} entries in db in {} ms.", param_n, elapsed),
    } as Response);
}

async fn get_first_values(
    state: axum::extract::State<AppState>,
    params: Query<HashMap<String, String>>,
) -> Json<Response> {
    let param_n: u32 = match params.get(&"n".to_string()).and_then(|n| n.parse::<u32>().ok()) {
        None => return Json(Response::new_error("Missing or invalid parameter n.")),
        Some(n) => n
    };

    let start_time = SystemTime::now();

    let mut fetched_data = state.db.get_entries().await;

    fetched_data.sort_by(|a, b| a.value.partial_cmp(&b.value).unwrap());

    let elapsed = start_time.elapsed().unwrap().as_millis() as u32;


    return Json(Response{
        data: Some(fetched_data.into_iter().take(param_n as usize).collect()),
        status: Status::SUCCESS,
        message: format!("Read {} entries from db in {} ms.", param_n, elapsed),
    })
}
