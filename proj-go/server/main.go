package main

import (
	"fmt"
	"log"
	"math/rand"
	"sort"
	"strconv"
	"time"

	"github.com/gofiber/fiber/v2"
)

func generateData(N int) []DataEntry {
	data := make([]DataEntry, N)

	for i := 0; i < N; i++ {
		K := rand.Intn(100000)

		data[i] = DataEntry{
			Name:  "name" + fmt.Sprint(10000000-i),
			Value: "value" + fmt.Sprint(K) + fmt.Sprint(i),
		}
	}
	return data
}

type ResponseStatus string

const (
	SUCCESS ResponseStatus = "success"
	ERROR   ResponseStatus = "error"
)

type Response struct {
	Status  ResponseStatus `json:"status"`
	Message string         `json:"message"`
	Data    []DataEntry    `json:"data"`
}

func currentUnixTime() int64 {
	return time.Now().UnixNano() / int64(time.Millisecond)
}

func main() {
	app := fiber.New()
	db := DbConnect()

	app.Get("/", func(c *fiber.Ctx) error {
		return c.SendString("Hello World")
	})

	app.Get("/reset_and_insert_data", func(c *fiber.Ctx) error {
		paramN, err := strconv.Atoi(string(c.Request().URI().QueryArgs().Peek("n")))
		if err != nil {
			return c.JSON(Response{
				Status:  ERROR,
				Message: "Missing or invalid parameter n.",
			})
		}

		startTimeUnix := currentUnixTime()

		DbReset(db)
		data := generateData(paramN)
		for _, entry := range data {
			DbInsertEntry(db, entry.Name, entry.Value)
		}

		duration := currentUnixTime() - startTimeUnix

		return c.JSON(Response{
			Status:  SUCCESS,
			Message: fmt.Sprintf("Inserted %d entries in db in %d ms.", paramN, duration),
		})
	})

	app.Get("/get_first_values", func(c *fiber.Ctx) error {
		paramN, err := strconv.Atoi(string(c.Request().URI().QueryArgs().Peek("n")))
		if err != nil {
			return c.JSON(Response{
				Status:  ERROR,
				Message: "Missing or invalid parameter n.",
			})
		}

		startTimeUnix := currentUnixTime()

		entries := DbGetEntries(db)
		sort.Slice(entries, func(i, j int) bool {
			return entries[i].Value < entries[j].Value
		})

		duration := currentUnixTime() - startTimeUnix

		return c.JSON(Response{
			Status:  SUCCESS,
			Message: fmt.Sprintf("Read %d entries from db in %d ms.", paramN, duration),
			Data:    entries[0:paramN],
		})
	})

	log.Fatal(app.Listen(":3002"))
}
