package main

// postgres
import (
    "github.com/go-pg/pg/v10"
)


type DataEntry struct {
    Name string `json:"name"`
    Value string `json:"value"`
}

func DbConnect() *pg.DB {
    return pg.Connect(&pg.Options{
        PoolSize: 10,
        Addr:     "localhost:5432",
        User:     "postgres",
        Password: "postgres",
        Database: "postgres",
    })
}

func DbReset(db *pg.DB) {
    _, err := db.Exec(`DROP TABLE IF EXISTS data`)
    if err != nil { panic(err) }

    _, err = db.Exec(`CREATE TABLE data (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100) NOT NULL,
        value VARCHAR(100) NOT NULL
    )`)
    if err != nil { panic(err) }
}

func DbInsertEntry(db *pg.DB, name string, value string) {
    _, err := db.Exec(`INSERT INTO data (name, value) VALUES (?, ?)`, name, value)
    if err != nil {
        panic(err)
    }
}

func DbGetEntries(db *pg.DB) []DataEntry {
    var data []DataEntry
    // raw select
    _, err := db.Query(&data, `SELECT name, value FROM data`)

    if err != nil {
        panic(err)
    }

    return data
}