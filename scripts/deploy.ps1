# Script de deploy para criar releases
param(
    [string]$Version = "2.0.0",
    [string]$Platform = "Windows"
)

Write-Host "🚀 Deploying Snake Game v$Version for $Platform" -ForegroundColor Green

# Build para release
.\scripts\build.bat Release

# Criar pacote ZIP
$ReleaseDir = "releases/v$Version"
New-Item -ItemType Directory -Force -Path $ReleaseDir

Copy-Item "build/bin/SnakeGame.exe" -Destination "$ReleaseDir/"
Copy-Item "assets" -Destination "$ReleaseDir/" -Recurse

# Criar ZIP
Compress-Archive -Path "$ReleaseDir/*" -DestinationPath "SnakeGame-v$Version-$Platform.zip"

Write-Host "✅ Release package created!" -ForegroundColor Green