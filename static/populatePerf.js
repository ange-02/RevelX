datas = []
label = []
for (let i = 0; i < 100; i++) {
    if ((i % 73 == 0 || i % 73 == 1 || i % 42 == 1 || i % 42 == 0) && (i != 0 && i != 1)) {
        min = 200
        max = 300
        bias = 0
        influence = 1.00
        var rnd = Math.random() * (max - min) + min,   // random in range
            mix = Math.random() * influence;           // random mixer 
        datas.push(rnd * (1 - mix) + bias * mix)
        label.push(i)
    } else {
        datas.push(Math.random() * 10 + 50)
        label.push(i)
    }
}
    
         
    


  

  const data = {
    labels: label,
    datasets: [{
      label: 'Reads per second',
      data: datas,
      fill: false,
      borderColor: 'rgb(75, 192, 192)',
      tension: 0.1
    }]
  };

  const config = {
    type: 'line',
    data: data,
    options: {
        responsive: false
    }
  };

  const chart = new Chart(document.getElementById("perfCanvas"), config)