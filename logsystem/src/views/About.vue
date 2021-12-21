<template>
  <div class="about">
    <SideMenu class="about-left"></SideMenu>
    <div class="about-right">
      <el-row class="home" :gutter="20">
        <el-col :span="8">
          <el-card shadow="hover" style="height: 200px">
            <div class="total">
              <div>
                <p style="line-height: 0px; font-size: 10px">所有日志</p>
                <p style="line-height: 0px; font-size: 30px">
                  {{ this.lsTotal }}
                </p>
                <p style="color: green">
                  ▲{{ this.lsTotal - this.total }}({{
                    Math.floor(
                      ((this.lsTotal - this.total) / this.total) * 10000
                    ) / 100
                  }}%)
                </p>
              </div>
              <div style="border: 1px solid #000; height: 110px" />
              <div>
                <p style="line-height: 0px; font-size: 10px">缓存日志</p>
                <p style="line-height: 0px; font-size: 30px">
                  {{ this.lsTotal }}
                </p>
                <p style="color: green">
                  ▲{{ this.lsTotal - this.total }}({{
                    Math.floor(
                      ((this.lsTotal - this.total) / this.total) * 10000
                    ) / 100
                  }}%)
                </p>
              </div>
            </div>
          </el-card>
          <el-card shadow="hover" style="height: 300px; margin-top: 20px">
            <ve-line
              height="300px"
              :data="chartData"
              :colors="colors"
              :extend="chartExtend"
              :settings="chartSettings"
            ></ve-line>
          </el-card>
          <el-card shadow="hover" style="height: 300px; margin-top: 20px">
            <ve-pie :data="pieData" :settings="pieSettings" :extend="pieExtend" ></ve-pie>
          </el-card>
        </el-col>
        <el-col :span="16">
          <!--显示流程图-->
          <el-card shadow="hover">
            <div style="height: 280px"></div>
          </el-card>
          <!--左边最下面两个流程图-->
          <div class="graph">
            <el-card shadow="hover" style="height: 400px; margin-top: 20px">
              <ve-histogram height="400px" :data="histogramData" :settings="histogramSettings" :extend="histogramExtend">
              </ve-histogram>
            </el-card>
          </div>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
import SideMenu from "./SideMenu.vue";
import "echarts/lib/component/title";
import {login} from '@/request/user.js'
export default {
  name: "about",
  components: {
    SideMenu,
  },
  data() {
    this.colors = ["#87CEFA"];
    this.chartExtend = {
      series: {
        smooth: false,
      },
      legend: {
        show: false,
      },
      title: {
        text: "日志趋势",
        x: "left", //在图片中的x轴位置 left, right, middle
        y: "top", //在图片中的y轴位置 top, bottom
        textAlign: "auto", //整体（包括 text 和 subtext）的水平对齐 auto, left, right, center
        textStyle: {
          color: "#000000",
          fontWeight: "normal",
          fontSize: 12,
        },
      },
    };
    this.chartSettings = {
      area: true,
      scale: [true],
    };
    this.pieSettings= { 
      type: 'pie' ,

      };
    this.pieExtend={
      series: {
          center: ['30%', '30%'],
          radius: '40%',
          minAngle:15,
        },
        legend: {
                type: 'scroll',  //设置超出部分滚动
                orient: 'vertical',  //设置竖向排列
                show:true,          //是否显示，默认开启
                left: '70%',
                top: 70,
      }, 
        title: {
        text: "日志类别占比",
        x: "left", //在图片中的x轴位置 left, right, middle
        y: "top", //在图片中的y轴位置 top, bottom
        textAlign: "auto", //整体（包括 text 和 subtext）的水平对齐 auto, left, right, center
        textStyle: {
          color: "#000000",
          fontWeight: "normal",
          fontSize: 12,
        },
        
    }};
    this.histogramSettings={
      type: 'histogram',
    };
    this.histogramExtend={
      series: {
        barWidth : 30
        },
        legend: {
        show: false,
      },
      title: {
        text: "文件日志数量",
        x: "left", //在图片中的x轴位置 left, right, middle
        y: "top", //在图片中的y轴位置 top, bottom
        textAlign: "auto", //整体（包括 text 和 subtext）的水平对齐 auto, left, right, center
        textStyle: {
          color: "#000000",
          fontWeight: "normal",
          fontSize: 12,
        },
      },
    };
    return {
      timer: "",
      lsTotal: 0,
      total: 1,

      chartData: {
        columns: ["月份", "日志数"],
        rows: [
          { 月份: "1", 日志数: "0" },
          { 月份: "2", 日志数: "0" },
          { 月份: "3", 日志数: "0" },
          { 月份: "4", 日志数: "0" },
          { 月份: "5", 日志数: "0" },
        ],
      },
      pieData: {
        columns: ['key', 'value'],
        rows: [
          {key: "debug", value: "0"},
           {key: "info", value: "0"},
            {key: "warning", value: "0"},
             {key: "error", value: "0"},
        ]
      },
      histogramData: {
       columns: ['key', 'value'],
        rows: [],
      },
    };
  },
  mounted() {
  //  axios
  //           .get('/api/v1/todo')
  //           .then((res) => {
  //               console.log(res.data) //    返回接口响应结果
  //           })
    login().then((res) =>{
      console.log(res);
    })
    this.total = localStorage.getItem("total");
    if (this.total == null || this.total == '0') {
      this.total = 1;
    }
    this.lsTotal = 1;
    console.log(this.lsTotal,  this.total);
    const that = this;
    this.timer = setInterval(function () {
      if (that.$store.state.total > that.lsTotal) {
        //日志数
        that.lsTotal = that.$store.state.total;
        //月份折线图
        let today = new Date();
        for (var i = 0; i < 5; i++) {
          let month =
            (today.getMonth() + 13 - 4 + i) % 12 == 0
              ? 12
              : (today.getMonth() + 13 - 4 + i) % 12;
          that.$set(that.chartData.rows, i, {
            月份: month + "",
            日志数: that.$store.state.monthTotal[month - 1] + "",
          });
        }
        //类别饼图
        for (i = 0; i < 4; i++) {
          that.$set(that.pieData.rows[i], "value", that.$store.state.levelTotal[i] + "");
        }
        //文件柱状图
        for (i = that.histogramData.rows.length; i < that.$store.state.fileTotal.length; i++) {
        that.histogramData.rows.push({key:that.$store.state.fileTotal[i].title, value: "0"});
      }
        for (i = 0; i < that.$store.state.fileTotal.length; i++) {
          that.$set(that.histogramData.rows[i], "value", that.$store.state.fileTotal[i].total + "");
        }
      }
    }, 100);
  },
  beforeDestroy() {
    clearInterval(this.timer);
  },
};
</script>

<style>
.home .total {
  display: flex; /*对于一行会自动弹性布局*/
  justify-content: space-between; /*表示水平两端对齐，justify-content：水平对齐的方式*/
}
.home .num {
  display: flex; /*对于一行会自动弹性布局*/
  flex-wrap: wrap; /*对于6个模块需要换行*/
  justify-content: space-between; /*表示水平两端对齐，justify-content：水平对齐的方式*/
}

.home .num .el-card {
  width: 32%;
  margin-bottom: 20px; /*模块之前有间隔*/
}

.home .graph {
  display: flex; /*对于一行会自动弹性布局*/
  justify-content: space-between; /*表示水平两端对齐，justify-content：水平对齐的方式*/
}

.home .graph .el-card {
  width: 100%;
  margin-top: 20px; /*模块之前有间隔*/
}
</style>
<style lang="stylus" scoped>
.about {
  // 使用flex布局 菜单居左固定宽度 右侧内容区域自适应宽度
  display: flex;

  .about-left {
    box-sizing: border-box;
  }

  .about-right {
    box-sizing: border-box;
    width: calc(100%);
    padding: 8px 24px;
    flex: 1;
    height: 750px;
  }
}
</style>