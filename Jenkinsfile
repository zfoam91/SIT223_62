pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'g++ -O main.cpp lib.cpp -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system'
            }
        }
        stage('Test') {
            steps {
                sh 'g++ -std=c++11 -o test_minesweeper test_minesweeper.cpp lib.cpp -lgtest -lgtest_main -pthread'
                sh './test_minesweeper'
            }
        }
        stage('Code Quality Analysis') {
            steps {
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 *.cpp *.hpp 2> cppcheck_report.xml'
            }
        }
        stage('Deploy') {
	    steps {
		sh 'docker build -t minesweeper .'
		sh 'xhost +local:docker'
		sh 'docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix minesweeper'
	    }
	}
    }
    post {
        always {
            junit 'test_results.xml'
            recordIssues enabledForFailure: true, tool: cppCheck(pattern: 'cppcheck_report.xml')
        }
    }
}
