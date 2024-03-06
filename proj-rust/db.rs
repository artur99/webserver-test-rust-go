use deadpool_postgres::{Config, Pool, Runtime};
use tokio_pg_mapper::FromTokioPostgresRow;
use tokio_pg_mapper_derive::PostgresMapper;
use tokio_postgres::NoTls;
use serde::{Serialize};

#[derive(Clone, Debug, PartialEq, Serialize, PostgresMapper)]
#[pg_mapper(table = "data")]
pub struct DataEntry {
    pub name: String,
    pub value: String,
}

#[derive(Clone)]
pub struct Database {
    pub pool: Pool,
}


impl Database {
    pub fn new() -> Database {
        let mut cfg = Config::new();
        let db_host = std::env::var("DB_HOST").unwrap_or("localhost".to_string());

        cfg.host = Some(db_host);
        cfg.user = Some("postgres".to_string());
        cfg.password = Some("postgres".to_string());
        cfg.dbname = Some("postgres".to_string());

        let pool = cfg.create_pool(Some(Runtime::Tokio1), NoTls).unwrap();
        pool.resize(10);

        Database { pool }
    }

    pub async fn reset_db(&self) {
        let client = self.pool.get().await.unwrap();
        client.simple_query("DROP TABLE IF EXISTS data").await.unwrap();
        client.simple_query("CREATE TABLE data (
            id SERIAL PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            value VARCHAR(100) NOT NULL
        )").await.unwrap();
    }

    pub async fn insert_entry(&self, name: &str, value: &str) {
        let client = self.pool.get().await.unwrap();
        let stmt = client.prepare_cached("INSERT INTO data(name, value) VALUES ($1, $2)").await.unwrap();
        client.query(&stmt, &[&name, &value]).await.unwrap();
    }

    pub async fn get_entries(&self) -> Vec<DataEntry> {
        let client = self.pool.get().await.unwrap();
        let result = client.query("SELECT name, value FROM data", &[]).await.unwrap();

        let mut data = Vec::<DataEntry>::with_capacity(result.capacity());

        for row in result {
            data.push(DataEntry::from_row(row).unwrap());
        }

        data
    }

 }