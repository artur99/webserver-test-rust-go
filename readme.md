## Some tech exploration 🗺️ 🕵🏻 ✨
Simple test to explore new tech and compare languages.

My hope was that I'd write good Rust, but up to this point, my Go is faster than my Rust, something's fishy. 🐟🥹

I'm open to suggestions on how to fix my Rust code to make it faster.

## Starting
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

## Testing
1. Either using the test calls defined in `./test.http`
2. Or what I used for benchmarking, cassowary, seems good. Install it with `npm i -g cassowary` (might need sudo) and then, for example:
```bash
cassowary run -u "http://127.0.0.1:3001/get_first_values?n=5000"
cassowary run -u "http://127.0.0.1:3002/get_first_values?n=5000"
cassowary run -u "http://127.0.0.1:3003/get_first_values?n=5000"
```

## Current results
### Node:
```
Starting Load Test with 1000 requests using 5 concurrent users

 100% [========================================] 10.185520375s


 TCP Connect.....................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=49.99ms     Median=49.00ms  p(95)=75.00ms
 Content Transfer................: Avg/mean=0.01ms      Median=0.00ms   p(95)=0.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 98.18
```

### Go:
```
Starting Load Test with 1000 requests using 5 concurrent users

 100% [========================================] 3.252455333s


 TCP Connect.....................: Avg/mean=0.20ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=15.63ms     Median=15.00ms  p(95)=19.00ms
 Content Transfer................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 307.46
```

### Rust:
```
Starting Load Test with 1000 requests using 5 concurrent users

 100% [========================================] 4.112901291s


 TCP Connect.....................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=19.92ms     Median=19.00ms  p(95)=29.00ms
 Content Transfer................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 243.14
```