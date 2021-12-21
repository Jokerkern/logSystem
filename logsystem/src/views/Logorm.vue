<template>
  <div class="Logorm">
    <SideMenu class="Logorm-left"></SideMenu>
    <div class="Logorm-right">
      <el-row
        ><el-col :span="8">
          <el-select v-model="value" placeholder="请选择" >
            <el-option
              v-for="item in options"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            >
            </el-option> </el-select
        ></el-col>
        <el-col :span="12"
          ><el-input v-if="value==='query'" v-model="input" placeholder="请输入内容"></el-input
        ></el-col>
        <el-col :span="4"
          ><el-button type="submit" v-if="value==='query'" @click="check">确认</el-button></el-col
        ></el-row
      >
    </div>
  </div>
</template>

<script>
import SideMenu from "./SideMenu.vue";
import {Query} from "@/request/user.js";
export default {
  name: "Logorm",
  components: {
    SideMenu,
  },

  data() {
    return {
      options: [
        {
          value: "add",
          label: "添加",
        },
        {
          value: "query",
          label: "查询",
        },
        {
          value: "delete",
          label: "删除",
        },
        {
          value: "modify",
          label: "修改",
        },
      ],
      value: "",
      input: "",
    };
  },
  methods:{
    check() {
        //var data = "/"+this.input;
        console.log(this.input);
        Query({id: this.input}).then((res) =>{
            console.log(res);
        })
    }
},
};




</script>

<style lang="stylus" scoped>
.Logorm {
  display: flex;

  .Logorm-left {
    box-sizing: border-box;
  }

  .Logorm-right {
    box-sizing: border-box;
    width: calc(100% - 200px);
    padding: 8px 24px;
    flex: 1;
  }
}
</style>