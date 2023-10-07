const express = require('express');
const app = express();
const pool = require('./db');
var bodyParser = require('body-parser');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use('/', express.static('./'));

const create_database = `CREATE DATABASE IF NOT EXISTS users`

const use_database = `USE users`

const create_table = `CREATE TABLE IF NOT EXISTS users (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	PRIMARY KEY (id)
)`;


const default_name =`INSERT INTO users (name) VALUES ('Shreya')`;


pool.getConnection().then(async (connection) => {
		try {
			await connection.query(create_database);
			await connection.query(use_database);
			await connection.query(create_table);
			await connection.query(default_name);
			connection.commit();
		} catch (err) {
			console.error('Error Creating Table:', err);
		} finally {
			connection.close()
		} 
});

app.get('/', (req, res) => {
	res.sendFile(__dirname + '/index.html');
})

app.get('/changeName', (req, res) => {
	res.sendFile(__dirname + '/changename.html');
});

app.post('/changeName',(req,res)=>{
	pool.getConnection().then(async(connection)=>{
		try{
			console.log(req);
			const newName=req.body.newName;
			console.log(newName);
			const id=1;
			await connection.query(use_database);
			await connection.query("UPDATE users SET name=? WHERE id=?",[newName,id]);
			connection.commit();
			res.redirect("/");
		}catch(err){
			console.log("Error updating name:",err);
			res.status(500).send("Error updating name");
		}finally{
			connection.close();
		}
	})
})

app.get('/set-name', async (req, res) => {
	pool.getConnection().then(async(connection)=>{
		try{
			const userId = 1; 
			await connection.query(use_database);
			const result = await connection.query('SELECT name FROM users WHERE id=?', [userId]);
			const userName = result[0].name;
			res.json({ name: userName });
		}catch(err){
			console.log("Error updating name:",err);
			res.status(500).send("Error updating name");
		}finally{
			connection.close();
		}
	})

  });


const port  = process.env.PORT || 8081;
console.log(port)
app.listen(port, ()=>{
		console.log("started");
});
