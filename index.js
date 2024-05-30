const express = require('express');
const open = require('open');
const path = require('path');
const userAddon = require('./build/Release/addon');

const app = express();
const PORT = 3000;

app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/check', (req, res) => {
    const username = req.body.username;
    const privilege = userAddon.getUserPrivilege(username);
    if (privilege !== "User not found") {
        res.redirect(`/result?username=${encodeURIComponent(username)}&privilege=${encodeURIComponent(privilege)}`);
    } else {
        res.send(`Пользователя ${username} нет`);
    }
});

app.get('/result', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'result.html'));
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
    open(`http://localhost:${PORT}`);
});