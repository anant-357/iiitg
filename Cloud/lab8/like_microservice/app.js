const express = require('express');
const app = express();
const pool = require('./db');
var bodyParser = require('body-parser');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use('/', express.static('./'));

const create_database = `CREATE DATABASE IF NOT EXISTS likes`

const use_database = `USE likes`

const create_table =`CREATE TABLE IF NOT EXISTS likes (
	id INT NOT NULL AUTO_INCREMENT,
	count INT NOT NULL DEFAULT 0,
	PRIMARY KEY (id)
  );  
`

const default_value =`INSERT INTO likes (count) VALUES (0)`;


pool.getConnection().then(async (connection) => {
		try {
			await connection.query(create_database);
			await connection.query(use_database);
			await connection.query(create_table);
			await connection.query(default_value);
			connection.commit();
		} catch (err) {
			console.error('Error Creating Table:', err);
		} finally {
			connection.close()
		} 
});


app.get('/like', (req, res) => {
	res.sendFile(__dirname + '/like.html');
});

app.get('/get-like-count', async (req, res) => {
	pool.getConnection().then(async (connection) => {
		try {
			await connection.query(use_database);
			const result = await connection.query('SELECT count FROM likes WHERE id = 1');
			const likeCount = result[0].count;
			res.json({ count: likeCount });
		} catch (err) {
			console.error('Error retrieving like count:', err);
			res.status(500).send('Error retrieving like count.');
		} finally {
			connection.release();
		}
	});
});

app.post('/increment-like-count', async (req, res) => {
	pool.getConnection().then(async (connection) => {
		try {
			await connection.query(use_database);
			await connection.query('UPDATE likes SET count = count + 1 WHERE id = 1');
			res.sendStatus(200);
		} catch (err) {
			console.error('Error incrementing like count:', err);
			res.status(500).send('Error incrementing like count.');
		} finally {
			connection.release();
		}
	});
});

const port  = process.env.PORT || 8083;
console.log(port)
app.listen(port, ()=>{
		console.log("started");
});
