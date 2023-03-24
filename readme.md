Simple test to explore new tech and compare languages.

My hope was that I'd write good Rust, but up to this point, my Go is faster than my Rust, something's fishy. 🐟🥹

## Starting:
1. Start the postgres db with docker, it will start with the default credentials.
```bash
docker-compose up -d
```

2. Starting the node server (`:3001`)
```bash
cd ./proj-node
npm i
npm start
```

2. Starting the go server (`:3002`)
```bash
cd ./proj-go
go run ./server
```

3. Starting the rust server (`:3003`)
```bash
cd ./proj-rust
cargo run
```

## Testing:
1. Either using the test calls defined in `./test.http`
2. Or what I used for benchmarking, cassowary, seems good. Install it with `npm i -g cassowary` (might need sudo) and then, for example:
```bash
cassowary run -u "http://127.0.0.1:3001/get_first_values?n=5000"
cassowary run -u "http://127.0.0.1:3002/get_first_values?n=5000"
cassowary run -u "http://127.0.0.1:3003/get_first_values?n=5000"
```