package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
	"net/http"
	"strconv"
)

const (
	JSON_SUCCESS int = 1
	JSON_ERROR   int = 0
)

type (
	todoLog struct {
		gorm.Model
		Logger string `json:"logger"`
		Time   string `json:"time"`
		Level  string `json:"level"`
		Log    string `json:"log"`
	}

	logTodo struct {
		ID     uint   `json:"id"`
		Logger string `json:"logger"`
		Time   string `json:"time"`
		Level  string `json:"level"`
		Log    string `json:"log"`
	}
)

func (todoLog) TableName() string {
	return "test1"
}

var db *gorm.DB

// 初始化
func init() {
	var err error
	var constr string
	constr = fmt.Sprintf("%s:%s@(%s:%d)/%s?charset=utf8mb4&parseTime=True&loc=Local", "root", "123456", "localhost", 3306, "test")
	db, err = gorm.Open("mysql", constr)
	if err != nil {
		panic("数据库连接失败")
	}

	db.AutoMigrate(&todoLog{})
}

func main() {

	r := gin.Default()
	v1 := r.Group("/api/v1/todo")
	{
		v1.POST("/", add)      // 添加新条目
		v1.GET("/", all)       // 查询所有条目
		v1.GET("/:id", take)   // 获取单个条目
		v1.PUT("/:id", update) // 更新单个条目
		v1.DELETE("/:id", del) // 删除单个条目
		v1.GET("/total", find) //查询条目总数
	}
	r.Run(":9089")
}

func add(c *gin.Context) {
	todo := todoLog{Logger: c.PostForm("logger"), Time: c.PostForm("time"), Level: c.PostForm("level"), Log: c.PostForm("log")}
	db.Save(&todo)
	c.JSON(http.StatusOK, gin.H{
		"status":     JSON_SUCCESS,
		"message":    "创建成功",
		"resourceId": todo.ID,
	})
}

func all(c *gin.Context) {

	var todos []todoLog
	var _todos []logTodo
	todoID := c.Query("id")
	db.Find(&todos, todoID)

	// 没有数据
	if len(todos) <= 0 {
		c.JSON(http.StatusOK, gin.H{
			"status":  JSON_ERROR,
			"message": "没有数据",
		})
		return
	}

	// 格式化
	for _, item := range todos {
		_todos = append(_todos, logTodo{
			ID:     item.ID,
			Logger: item.Logger,
			Time:   item.Time,
			Level:  item.Level,
			Log:    item.Log,
		})
	}

	c.JSON(http.StatusOK, gin.H{
		"status":  JSON_SUCCESS,
		"message": "ok",
		"data":    _todos,
	})

}

func take(c *gin.Context) {
	var todo todoLog
	todoID := c.Param("id")

	db.First(&todo, todoID)
	if todo.ID == 0 {
		c.JSON(http.StatusNotFound, gin.H{
			"status":  JSON_ERROR,
			"message": "条目不存在",
		})
		return
	}

	_todo := logTodo{
		ID:     todo.ID,
		Logger: todo.Logger,
		Time:   todo.Time,
		Level:  todo.Level,
		Log:    todo.Log,
	}
	c.JSON(http.StatusOK, gin.H{
		"status":  JSON_SUCCESS,
		"message": "ok",
		"data":    _todo,
	})
}

func update(c *gin.Context) {
	var todo todoLog
	todoID := c.Param("id")
	db.First(&todo, todoID)
	if todo.ID == 0 {
		c.JSON(http.StatusNotFound, gin.H{
			"status":  JSON_ERROR,
			"message": "条目不存在",
		})
		return
	}

	db.Model(&todo).Update("title", c.PostForm("title"))
	completed, _ := strconv.Atoi(c.PostForm("completed"))
	db.Model(&todo).Update("completed", completed)
	c.JSON(http.StatusOK, gin.H{
		"status":  JSON_SUCCESS,
		"message": "更新成功",
	})
}
func del(c *gin.Context) {
	var todo todoLog
	todoID := c.Param("id")
	db.First(&todo, todoID)
	if todo.ID == 0 {
		c.JSON(http.StatusOK, gin.H{
			"status":  JSON_ERROR,
			"message": "条目不存在",
		})
		return
	}
	db.Delete(&todo)
	c.JSON(http.StatusOK, gin.H{
		"status":  JSON_SUCCESS,
		"message": "删除成功!",
	})
}
func find(c *gin.Context) {
	var todo todoLog
	result := db.Find(&todo)
	if result != nil {
		c.JSON(http.StatusOK, gin.H{
			"status":         JSON_SUCCESS,
			"messages_total": result.RowsAffected,
		})
		return
	} else {
		c.JSON(http.StatusOK, gin.H{
			"status":        JSON_ERROR,
			"message_total": result.Error,
		})
		return
	}

}
