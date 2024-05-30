@echo off
call npm install
call npm run build
node index.js
pause