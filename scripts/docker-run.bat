@echo off
REM Build sem cache
docker compose build --no-cache
IF ERRORLEVEL 1 EXIT /B 1

REM Roda modo texto
docker compose run --rm snake-text
