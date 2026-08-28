#!/bin/bash
# Script de build para Unix-like systems

set -e  # Para em caso de erro

echo "🐍 Building Snake Game..."

# Configurações
BUILD_TYPE=${1:-Release}
JOBS=${2:-4}

# Criar diretório de build
mkdir -p build
cd build

# Configurar CMake
cmake -B . -S .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Compilar
echo "🔨 Compiling with $JOBS jobs..."
cmake --build . --config $BUILD_TYPE --parallel $JOBS

echo "✅ Build completed successfully!"
echo "🎮 Run with: ./bin/SnakeGame"