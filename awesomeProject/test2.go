//func main() {
//	dsn := "root:123456@tcp(127.0.0.1:3306)/test?charset=utf8mb4&parseTime=True&loc=Local"
//	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
//	if err != nil {
//		panic("failed to connect database")
//	}
//
//	// Migrate the schema
//	db.AutoMigrate(&Product{})
//
//	// Create
//	db.Create(&Product{Code: "D42", Price: 100})
//
//	// Read
//	var product Product
//	db.First(&product, 1)                 // find product with integer primary key
//	db.First(&product, "code = ?", "D42") // find product with code D42
//
//	// Update - update product's price to 200
//	db.Model(&product).Update("Price", 200)
//	// Update - update multiple fields
//	db.Model(&product).Updates(Product{Price: 200, Code: "F42"}) // non-zero fields
//	db.Model(&product).Updates(map[string]interface{}{"Price": 200, "Code": "F42"})
//
//	// Delete - delete product
//	db.Delete(&product, 1)
//	fmt.Println("end")
//}

package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"fmt"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
	"net"
	"strings"
	"time"
)

type todoLog struct {
	gorm.Model
	Logger string `json:"logger"`
	Time   string `json:"time"`
	Level  string `json:"level"`
	Log    string `json:"log"`
}

func (todoLog) TableName() string {
	return "test1"
}

func main() {
	listen, err := net.Listen("tcp", "127.0.0.1:9999")
	if err != nil {
		fmt.Println("Listen() failed, err: ", err)
		return
	}
	for {
		conn, err := listen.Accept() // 监听客户端的连接请求
		if err != nil {
			fmt.Println("Accept() failed, err: ", err)
			continue
		}
		go Process(conn) // 启动一个goroutine来处理客户端的连接请求
	}
}

func Process(conn net.Conn) {
	defer conn.Close()
	dsn := "root:123456@tcp(127.0.0.1:3306)/test?charset=utf8mb4&parseTime=True&loc=Local"
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		fmt.Println("gorm失败，err:", err)
	}
	err = db.AutoMigrate(&todoLog{})
	if err != nil {
		return
	}
	ticker := time.NewTicker(time.Second * 5)
	log := make([]todoLog, 0)
	go func() {
		for {
			select {
			case <-ticker.C:
				length := len(log)
				if length == 0 {
					continue
				}
				db.Create(&log)
				log = log[length:]
				//fmt.Println(log)
			}
		}
	}()
	for {
		reader := bufio.NewReader(conn)
		msg, err := Decode(reader)
		fmt.Println(msg)
		if err != nil {
			return
		}
		arr := strings.Fields(msg)
		//fmt.Println(arr[0], arr[1], arr[2], arr[3])
		//ticker = time.NewTicker(time.Second * 5)

		log = append(log, todoLog{Logger: arr[2][1 : len(arr[2])-1], Time: arr[0][1:] + " " + arr[1][:len(arr[1])-1], Level: arr[3][1 : len(arr[3])-1], Log: msg[len(arr[0])+len(arr[1])+len(arr[2])+len(arr[3])+4:]})
		//fmt.Println("3")
		//db.Create(&todoLog{Logger: arr[2][1 : len(arr[2])-1], Time: arr[0][1:] + " " + arr[1][:len(arr[1])-1], Level: arr[3][1 : len(arr[3])-1], Log: msg[len(arr[0])+len(arr[1])+len(arr[2])+len(arr[3])+4:]})
		//if err == io.EOF {
		//	return
		//}

		fmt.Printf("%v %v %v %v\n", arr[0][1:]+" "+arr[1][:len(arr[1])-1], arr[2][1:len(arr[2])-1], arr[3][1:len(arr[3])-1], msg[len(arr[0])+len(arr[1])+len(arr[2])+len(arr[3])+4:])
	}
}

func Decode(reader *bufio.Reader) (string, error) {
	// 读消息长度
	lengthByte, _ := reader.Peek(4)
	lengthBuff := bytes.NewBuffer(lengthByte)

	var length int32
	err := binary.Read(lengthBuff, binary.LittleEndian, &length)
	if err != nil {
		return "", err
	}
	// buffer返回缓冲中现有的可读的字节数
	if int32(reader.Buffered()) < length+4 {
		return "", err
	}
	// 读取真正的数据
	//fmt.Println(length)
	pack := make([]byte, int(4+length))
	_, err = reader.Read(pack)
	if err != nil {
		return "", err
	}
	//fmt.Println(n)
	return string(pack[4:]), nil
}
