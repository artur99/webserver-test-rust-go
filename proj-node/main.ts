import { Server } from 'hyper-express';
import { Db, DataEntry } from './db';

const webserver = new Server();
const db = new Db();

const generateData = (N: number = 1000): Array<DataEntry> => {
    // new array of size N
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

webserver.get('/', (req, res) => {
    res.send('Hello World');
})

webserver.get('/reset_and_insert_data', async (req, res) => {
    const paramN = parseInt(req.query_parameters.n || '');
    if(isNaN(paramN)) {
        return res.json({ status: Status.ERROR, message: 'Missing or invalid parameter n.' } as Response);
    }

    const start = Date.now();

    const dataEntries = generateData(paramN);

    await db.resetDb();
    for (const entry of dataEntries) {
        await db.insertEntry(entry.name, entry.value);
    }

    const duration = Date.now() - start;

    res.json({
        status: Status.SUCCESS,
        message: `Inserted ${paramN} entries in db in ${duration} ms.`
    } as Response);
})

webserver.get('/get_first_values', async (req, res) => {
    const paramN = parseInt(req.query_parameters.n || '');
    if(isNaN(paramN)) {
        return res.json({ status: Status.ERROR, message: 'Missing or invalid parameter n.' } as Response);
    }

    const start = Date.now();

    const dataEntries = await db.getEntries();
    const result = dataEntries.sort((a, b) => a.value.localeCompare(b.value)).slice(0, paramN);

    const duration = Date.now() - start;

    res.json({
        status: Status.SUCCESS,
        message: `Read ${paramN} entries from db in ${duration} ms.`,
        data: result
    } as Response);
})




const PORT = 3001;
webserver
    .listen(PORT)
    .then((socket) => console.log(`Webserver started on port ${PORT}`))