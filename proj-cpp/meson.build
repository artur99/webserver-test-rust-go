project('server', 'cpp', version: '0.1')

pqxx_dep = dependency('libpqxx') # PostgreSQL library
websockets_dep = dependency('uwebsockets') # uWebSockets library

source_files = [
    'src/main.cpp',
    'src/Database.cpp'
]

executable('server', source_files, dependencies: [pqxx_dep, websockets_dep])
