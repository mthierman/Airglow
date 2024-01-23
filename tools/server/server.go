package main

import (
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", serveIndex)
	serveAssets()

	log.Fatal(http.ListenAndServe("localhost:3333", nil))
}

func serveIndex(w http.ResponseWriter, r *http.Request) {
	index := "./gui/index.html"
	http.ServeFile(w, r, index)
}

func serveAssets() {
	fs := http.FileServer(http.Dir("./gui/assets/"))
	http.Handle("/assets/", http.StripPrefix("/assets/", fs))
}
