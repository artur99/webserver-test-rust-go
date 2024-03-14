import { Hono } from 'hono'
import { Db, DataEntry } from './db';

const app = new Hono()
const db = new Db();

const generateData = (N: number = 1000): Array<DataEntry> => {
    const data = new Array<DataEntry>(N);

    for (let i = 0; i < N; i++) {
        const K = Math.floor(Math.random() * 100000);
        data[i] = {
            name: `name${10000000 - i}`,
            value: `value${K}${i}`
        };
    };

    return data;
}

enum Status {
    SUCCESS = 'success',
    ERROR = 'error'
}

interface Response {
    status: Status,
    message: string,
    data: any,
}

app.get('/', (c) => {
    return c.text('Hello World')
})

app.get('/reset_and_insert_data', async (c) => {
    const paramN = parseInt(c.req.query('n') || '');
    if(isNaN(paramN)) {
        return c.json({ status: Status.ERROR, message: 'Missing or invalid parameter n.' } as Response);
    }

    const start = Date.now();

    const dataEntries = generateData(paramN);

    await db.resetDb();
    for (const entry of dataEntries) {
        await db.insertEntry(entry.name, entry.value);
    }

    const duration = Date.now() - start;

    return c.json({
        status: Status.SUCCESS,
        message: `Inserted ${paramN} entries in db in ${duration} ms.`
    } as Response);
})

app.get('/get_first_values', async (c) => {
    const paramN = parseInt(c.req.query('n') || '');
    if(isNaN(paramN)) {
        return c.json({ status: Status.ERROR, message: 'Missing or invalid parameter n.' } as Response);
    }

    const start = Date.now();

    const dataEntries = await db.getEntries();
    const result = dataEntries.sort((a, b) => a.value.localeCompare(b.value)).slice(0, paramN);

    const duration = Date.now() - start;

    return c.json({
        status: Status.SUCCESS,
        message: `Read ${paramN} entries from db in ${duration} ms.`,
        data: result
    } as Response);
})

export default {
    port: 3005,
    fetch: app.fetch,
}