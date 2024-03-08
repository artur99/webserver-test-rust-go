FROM rust:1.68

WORKDIR /app
ADD . /app

ENV CARGO_REGISTRIES_CRATES_IO_PROTOCOL=sparse
RUN cargo build --release

CMD ["/app/target/release/proj-rust"]