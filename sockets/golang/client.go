package main

import (
	"log"
	"net"

	"golang.org/x/sys/unix"
)

const (
	port    = 8080
	address = "127.0.0.1"
)

func main() {
	sock, err := unix.Socket(unix.AF_INET, unix.SOCK_STREAM, 0)
	checkErr(err)
	defer unix.Close(sock)

	serverAddr := &unix.SockaddrInet4{
		Port: port,
		Addr: [4]byte(net.ParseIP(address).To4()),
	}

	err = unix.Connect(sock, serverAddr)
	checkErr(err)
	msg := "hello from go"
	_, err = unix.Write(sock, []byte(msg))
	checkErr(err)
}

func checkErr(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
