echo "Running unit tests:"

for i in tests/*_tests
do
    if test -f $i
    then
        if $VALGRIND ./$i /home/kirill/learn_c/ex29/c-skeleton/build/libex29.so uppercase "uPPerst" 2>> tests/tests.log &&\
            ./$i /home/kirill/learn_c/ex29/c-skeleton/build/libex29.so lowercase "LOwErcase" 2>> tests/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's tests/tests.log"
            echo "------"
            tail tests/tests.log
            exit 1
        fi
    fi
done

echo ""
