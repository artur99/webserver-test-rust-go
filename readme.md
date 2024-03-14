## Some tech exploration 🗺️ 🕵🏻 ✨
Simple test to explore new tech and compare languages.

My hope was that I'd write good Rust, but up to this point, my Go is faster than my Rust, something's fishy. 🐟🥹

I'm open to suggestions on how to fix my Rust code to make it faster.

## Starting all
All services are set up with docker
```bash
docker-compose build
docker-compose up -d
```

## Starting manually
1. Start the postgres db with docker, it will start with the default credentials.
```bash
docker-compose up -d db
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

4. Starting the cpp server (`:3004`)
```bash
cd ./proj-cpp
./configure.sh
./build.sh
./run.sh
```

5. Starting the node server (`:3005`)
```bash
cd ./proj-bun
bun i
bun main.ts
```

## Testing
1. Either using the test calls defined in `./test.http`
2. Or what I used for benchmarking, cassowary, seems good. Install it with `npm i -g cassowary` (might need sudo) and then, for example:
```bash
cassowary run -u "http://127.0.0.1:3001/get_first_values?n=5000" -c 3 -n 1300
cassowary run -u "http://127.0.0.1:3002/get_first_values?n=5000" -c 3 -n 1300
cassowary run -u "http://127.0.0.1:3003/get_first_values?n=5000" -c 3 -n 1300
cassowary run -u "http://127.0.0.1:3004/get_first_values?n=5000" -c 3 -n 1300
cassowary run -u "http://127.0.0.1:3005/get_first_values?n=5000" -c 3 -n 1300
```

## Current results
### Node:
```
Starting Load Test with 1000 requests using 3 concurrent users

 100% [========================================] 12.1563595s


 TCP Connect.....................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=34.85ms     Median=34.00ms  p(95)=47.00ms
 Content Transfer................: Avg/mean=0.53ms      Median=1.00ms   p(95)=1.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 82.26
```

### Bun:
```
Starting Load Test with 1000 requests using 3 concurrent users

 100% [========================================] 10.44617075s


 TCP Connect.....................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=29.95ms     Median=29.00ms  p(95)=41.00ms
 Content Transfer................: Avg/mean=0.22ms      Median=0.00ms   p(95)=1.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 95.73
```

### Go:
```
Starting Load Test with 1000 requests using 3 concurrent users

 100% [========================================] 2.705078958s


 TCP Connect.....................: Avg/mean=0.33ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=6.95ms      Median=6.00ms   p(95)=10.00ms
 Content Transfer................: Avg/mean=0.10ms      Median=0.00ms   p(95)=1.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 369.67
```

### Rust:
```
Starting Load Test with 1000 requests using 3 concurrent users

 100% [========================================] 3.039154041s


 TCP Connect.....................: Avg/mean=0.00ms      Median=0.00ms   p(95)=0.00ms
 Server Processing...............: Avg/mean=8.00ms      Median=7.00ms   p(95)=12.00ms
 Content Transfer................: Avg/mean=0.09ms      Median=0.00ms   p(95)=1.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 329.04
```

### Cpp:
```
Starting Load Test with 1000 requests using 3 concurrent users

 100% [========================================] 1.280602057s


 TCP Connect.....................: Avg/mean=1.00ms 	Median=1.00ms	p(95)=1.00ms
 Server Processing...............: Avg/mean=5.75ms 	Median=5.00ms	p(95)=8.00ms
 Content Transfer................: Avg/mean=0.00ms 	Median=0.00ms	p(95)=0.00ms

Summary:
 Total Req.......................: 1000
 Failed Req......................: 0
 DNS Lookup......................: 0.00ms
 Req/s...........................: 780.88

```
