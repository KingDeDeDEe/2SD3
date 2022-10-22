// Muzamil Janjua

package main

import (
	"fmt"
	"os"
	"strconv"
)

const MAX int = 1000000

func doit(a []int64, from int, to int, c chan int64) {
	// TODO display a message of which portion of the array you are processing
	fmt.Println("doing ", from, " -- ", to)
	// TODO define and initialize ito 0 an int64 variable
	var sum int64 = 0
	// TODO in a for loop, traverse your portion of array, setting the values of
	// TODO the items to the value of the index+1 (i.e. the item with index 27 will have
	// TODO value of 28), and cumulate the total in the variable
	for i := from; i <= to; i++ {
		a[i] = int64(i) + 1
		sum = sum + a[i]
	}
	// TODO send the resulting value to the channel c
	c <- sum
}

func main() {
	// TODO define an array of size MAX of int64
	var a [MAX]int64
	// TODO check the number of command line ruments, if not 2, display error
	// TODO and terminate  -- you need the package os for that
	if len(os.Args) != 2 {
		fmt.Println("usage: program <number-of-goroutines>")
		return
	}
	// TODO convert the second command line argument to number_of_goroutines
	// TODO you need the package strconv for that
	// TODO if the second command line argument is not a number, error message and
	// TODO terminate
	nof_goroutines, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Wrong command line argument")
		return
	}
	// TODO if the second command line argument were a number, disoplay a message of'
	// TODO how many gooroutines is used
	fmt.Println("number of goroutines = ", nof_goroutines)
	// TODO compute the "slice" of the array for each goroutine
	// TODO and display it
	slice := MAX / nof_goroutines
	r := MAX % nof_goroutines
	fmt.Println("slice = ", slice)
	// initializes c as a channel
	c := make(chan int64)
	// TODO in a loop as long as there is a goroutine dispatched, dispatch the required
	// TODO number of goroutines with the info, receive their result, and sume the result
	// TODO together, and display the result
	from := 0
	to := 0
	var sum int64 = 0

	for i := 0; i < nof_goroutines; i++ {
		to += slice - 1
		if r > 0 {
			to += 1
			r = r - 1
			go doit(a[0:], from, to, c)
		} else {
			go doit(a[0:], from, to, c)
		}
		to += 1
		from = to
	}

	for i := 0; i < nof_goroutines; i++ {
		sum += <-c
	}

	fmt.Println(sum)

}
