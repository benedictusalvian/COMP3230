EXE=./tesla_factory.out

# n: number of cars to produce
# s: space
# a: num typeA robot
# b: num typeB robot
# c: num typeC robot

#      n s a b c
${EXE} 1 2 2 0 0
${EXE} 1 2 0 2 0
${EXE} 1 2 0 0 2

${EXE} 1 2 1 1 0
${EXE} 1 2 1 0 1
${EXE} 1 2 0 1 1

${EXE} 2 2 1 1 0
${EXE} 2 2 1 0 1
${EXE} 2 2 0 1 1

${EXE} 3 2 1 1 0
${EXE} 3 2 1 0 1
${EXE} 3 2 0 1 1

${EXE} 2 3 1 1 0
${EXE} 2 3 1 0 1
${EXE} 2 3 0 1 1

${EXE} 2 3 1 1 1
${EXE} 2 3 1 1 1
${EXE} 2 3 1 1 1

${EXE} 2 4 1 1 1
${EXE} 2 4 1 1 1
${EXE} 2 4 1 1 1

#... you can add more test cases 
