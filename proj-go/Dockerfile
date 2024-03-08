FROM golang:1.19-alpine

WORKDIR /app

COPY go.mod go.sum ./

RUN go mod download

ADD ./server ./server

RUN CGO_ENABLED=0 GOOS=linux go build -o ./build-server ./server/*.go

CMD ["/app/build-server"]