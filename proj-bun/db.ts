import postgres from 'postgres'

const sql = postgres({
    host: Bun.env['DB_HOST'] || 'localhost',
    port: 5432,
    database: 'postgres',
    username: 'postgres',
    password: 'postgres',
    max: 10,
});

export interface DataEntry {
    name: string,
    value: string
};

export class Db {
    public async resetDb() {
        await sql`DROP TABLE IF EXISTS data`;
        await sql`
            CREATE TABLE data (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100) NOT NULL,
                value VARCHAR(100) NOT NULL
            )`;
    }

    public async insertEntry(name: string, value: string) {
        await sql`
            INSERT INTO data (name, value)
            VALUES (${name}, ${value})`;
    }

    public async getEntries(): Promise<Array<DataEntry>> {
        const entries = await sql<DataEntry[]>`SELECT name, value FROM data`;
        return entries;
    }
}