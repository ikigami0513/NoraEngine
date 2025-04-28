# cmake/FindPythonCustom.cmake

# Ce script tente d'abord de trouver un Python dans le venv local,
# sinon il tombe sur le Python global du système.

message(STATUS "🔎 Recherche de Python pour le projet...")

# 1. Essayer d'utiliser un venv local (dans ${CMAKE_SOURCE_DIR}/venv)
if(EXISTS "${CMAKE_SOURCE_DIR}/venv/Scripts/python.exe" OR EXISTS "${CMAKE_SOURCE_DIR}/venv/bin/python")
    message(STATUS "✅ Environnement virtuel détecté.")
    
    if(WIN32)
        set(PYTHON_EXECUTABLE "${CMAKE_SOURCE_DIR}/venv/Scripts/python.exe")
    else()
        set(PYTHON_EXECUTABLE "${CMAKE_SOURCE_DIR}/venv/bin/python")
    endif()

    execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "import sys; print(sys.prefix)"
        OUTPUT_VARIABLE _python_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "📍 Python venv prefix: ${_python_prefix}")

    set(Python3_ROOT_DIR "${_python_prefix}")
endif()

# 2. Trouver Python (venv ou système)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

message(STATUS "🎯 Python trouvé: ${Python3_EXECUTABLE}")
message(STATUS "🧩 Python version : ${Python3_VERSION}")
message(STATUS "📚 Python includes : ${Python3_INCLUDE_DIRS}")
