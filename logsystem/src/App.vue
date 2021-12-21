<template>
  <div id="app">
    <router-view />
  </div>
</template>

<script>
export default {
  name: 'app',
  data() {
    return {
      webSocket: null,
      logArr: [],
    };
  },
  created() {
    this.initWebSocket();
  },
  mounted() {
    const loading = this.$loading({
          lock: true,
        });
    setTimeout(() => {
      setTimeout(() => {
        loading.close();
      }, this.$store.state.total / 2);
    }, 2000);
    const that = this;
     window.onbeforeunload = function () {
                window.localStorage.setItem('total', that.$store.state.total);
            }
  },
  destroyed() {
    this.webSocket.close();
  },
  beforeDestroy(){
  },
  close() {
   
  },
  methods: {
    initWebSocket() {
      //初始化weosocket
      let localhost = localStorage.getItem('ip_address');
      let port = localStorage.getItem("port");
      console.log(localhost, port);
      this.webSocket = new WebSocket("ws://" + localhost + ":" + port + "");
      this.webSocket.onopen = this.onOpen;
      this.webSocket.onmessage = this.onMessage;
      this.webSocket.onerror = this.onError;
      this.webSocket.onclose = this.onClose;
    },
    onMessage(event) {
      var log = this.cutString(event);
      if (!this.logArr.includes(log.file)) {
        this.logArr.push(log.file);
        this.$store.commit('changeLog', {
          title: log.file,
          content: [],
        });
        this.$store.commit('changeFile', {
          title: log.file,
          total: 0,
        });
      }
      var logIndex = this.logArr.indexOf(log.file);
      this.$store.commit('changeTotal');
      this.$store.commit('changeLogContent', {logIndex: logIndex, log: log});
      this.$store.commit('changeFileTotal', logIndex);
      this.$store.commit('changeMonthTotal', new Date(log.date).getMonth());
      switch(log.level) {
        case "debug":
          this.$store.commit('changeLevelTotal', 0);
          break;
        case "info":
          this.$store.commit('changeLevelTotal', 1);
          break;
        case "warning":
          this.$store.commit('changeLevelTotal', 2);
          break;
        case "error":
          this.$store.commit('changeLevelTotal', 3);
          break;
      }
    },
    cutString(event) {
      var str = new String();
      var arr = new Array();
      str = event.data;
      arr = str.split(" ");
      let len =
        arr[0].length + arr[1].length + arr[2].length + arr[3].length + 4;
      return {
        date: arr[0].slice(1, arr[0].length),
        time: arr[1].slice(0, arr[1].length - 1),
        file: arr[2].slice(1, arr[2].length - 1),
        level: arr[3].slice(1, arr[3].length - 1),
        message: str.slice(len),
      };
    },
    onOpen() {
      console.log("open");
    },
    onError() {
      this.initWebSocket();
    },
    onClose() {
      console.log("close");
    },
  },
}
</script>