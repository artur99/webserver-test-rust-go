FROM oven/bun:alpine

WORKDIR /app
COPY package.json /app
COPY bun.lockb /app

RUN bun i --production --no-save --frozen-lockfile --no-cache

COPY . /app

CMD ["bun", "main.ts"]