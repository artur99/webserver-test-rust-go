# FROM multiarch/qemu-user-static:x86_64-aarch64 as qemu

FROM node:19-alpine

# COPY --from=qemu /usr/bin/qemu-aarch64-static /usr/bin/
# RUN apk update
RUN apk add libc6-compat


WORKDIR /app
COPY package.json /app
COPY package-lock.json /app

RUN npm ci

COPY . /app

CMD ["npm", "start"]