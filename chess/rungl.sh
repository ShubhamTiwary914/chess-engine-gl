
inp="$1"

if [ -z "$inp" ]; then
    #default: run openGL project
    ./build.sh && ./build/OpenGLProject
else
    echo "test"
    ./build.sh && ./build/test_chess
fi