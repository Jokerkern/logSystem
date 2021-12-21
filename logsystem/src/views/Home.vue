<template>
  <div class="home">
    <!-- 3. 使用组件 -->
    <!-- 左侧菜单 - 区分文件类型 -->
    <SideMenu class="home-left"></SideMenu>
    <!-- 右侧内容区 -->
    <div class="home-right">
      <el-empty v-show="isConnected" description="连接不到websocket" class="empty"></el-empty>
      <el-tabs v-show="!isConnected" v-model="editableTabsValue" type="card">
        <el-tab-pane
          v-for="item in editableTabs"
          :key="item.name"
          :label="item.title"
          :name="item.name"
          align="left"
        >
          <el-row>
            <el-col :span="18">
              <el-date-picker
                v-model="value1"
                unlink-panels
                size="mini"
                type="daterange"
                range-separator="至"
                start-placeholder="开始日期"
                end-placeholder="结束日期"
              >
              </el-date-picker
            ></el-col>
            <el-col :span="4"
              ><el-input
                v-model="input"
                size="mini"
                placeholder="输入关键字搜索"
                @change="handleSearch(item)"
              >
                <el-button
                  slot="append"
                  icon="el-icon-search"
                  @click="handleSearch"
                ></el-button> </el-input
            ></el-col>
          </el-row>

          <el-table
            :data="
              item.content
                .filter(
                  (data) =>
                    !search ||
                    data.message.toLowerCase().includes(search.toLowerCase())
                )
                .filter(
                  (data) =>
                    !value1 ||
                    (new Date(data.date).getTime() - 28800000 >=
                      value1[0].getTime() &&
                      new Date(data.date).getTime() - 28800000 <=
                        value1[1].getTime())
                )
                .slice(
                  (item.currentPage - 1) * pagesize,
                  item.currentPage * pagesize
                )
            "
          >
            <el-table-column
              prop="date"
              label="日期"
              width="250"
              align="center"
            ></el-table-column>
            <el-table-column
              prop="time"
              label="时间"
              width="250"
              align="center"
            ></el-table-column
            ><el-table-column
              prop="file"
              label="文件"
              width="250"
              align="center"
            ></el-table-column
            ><el-table-column
              prop="level"
              label="类别"
              column-key="date"
              width="200"
              align="center"
              :filters="[
                { text: 'info', value: 'info' },
                { text: 'debug', value: 'debug' },
                { text: 'warning', value: 'warning' },
                { text: 'error', value: 'error' },
              ]"
              :filter-method="filterHandler"
            ></el-table-column>
            <el-table-column
              prop="message"
              label="内容"
              width="400"
              align="center"
            ></el-table-column>
          </el-table>
          <el-pagination
            @size-change="handleSizeChange"
            @current-change="handleCurrentChange(item, $event)"
            :current-page.sync="item.currentPage"
            :page-sizes="[5, 10, 20, 40]"
            :page-size="pagesize"
            layout="total, sizes, prev, pager, next, jumper"
            :total="
              item.content
                .filter(
                  (data) =>
                    !search ||
                    data.message.toLowerCase().includes(search.toLowerCase())
                )
                .filter(
                  (data) =>
                    !value1 ||
                    (new Date(data.date).getTime() - 28800000 >=
                      value1[0].getTime() &&
                      new Date(data.date).getTime() - 28800000 <=
                        value1[1].getTime())
                ).length
            "
            align="center"
          >
            //这是显示总共有多少数据
          </el-pagination>
        </el-tab-pane>
      </el-tabs>
    </div>
  </div>
</template>

<script>
import SideMenu from "./SideMenu.vue"; //  1.引入左侧菜单组件
export default {
  name: "home",
  components: {
    SideMenu,
  },
  data() {
    return {
      editableTabsValue: "0",
      editableTabs: [],
      tabIndex: 0,
      logArr: [],
      content: [],
      timer: "",
      pagesize: 10, //    每页的数据
      search: "",
      input: "",
      value1: "",
      isConnected: true,
    };
  },
  created() {
  },
  mounted() {
    const that = this;
    this.timer = setInterval(function () {
      let log = that.$store.state.log;
      //console.log(log[0].content);
      for (var i = that.logArr.length; i < log.length; i++) {
        that.logArr.push(log[i].title);
        let newTabName = ++that.tabIndex + "";
        that.editableTabs.push({
          currentPage: 1, //初始页
          title: log[i].title,
          name: newTabName,
          content: [],
        });
        that.editableTabsValue = newTabName;
        that.isConnected = false;
      }
      for (i = 0; i < log.length; i++) {
        that.editableTabs[i].content = [].concat(log[i].content);
        //console.log(that.content[i][j]);
        //log[i].content = [];
      }
    }, 100);
  },
  beforeDestroy() {
    clearInterval(this.timer);
  },
  destroyed() {
    //this.webSocket.close(); //离开路由之后断开websocket连接
  },
  methods: {
    handleSizeChange: function (size) {
      this.pagesize = size;
      //console.log(size); //每页下拉显示数据
    },
    handleCurrentChange: function (item, event) {
      item.currentPage = event; //每页下拉显示数据
    },
    handleSearch: function () {
      this.search = this.input;
      //console.log(this.value1[0].getTime())
    },
    filterHandler(value, row, column) {
      const property = column["property"];
      return row[property] === value;
    },
  },
};
</script>
<style lang="stylus" scoped>
.home {
  // 使用flex布局 菜单居左固定宽度 右侧内容区域自适应宽度
  display: flex;

  .home-left {
    box-sizing: border-box;
  }

  .home-right {
    box-sizing: border-box;
    width: calc(100% - 200px);
    padding: 8px 24px;
    flex: 1;
  }

  .empty{
    text-align: center;
    position: absolute;
    top: 50%;
    left: 55%;
    transform: translate(-50%,-55%);
  }
}
</style>

