function createPlot(data, title) {
  var x = [];
  var y1 = [];
  var y2 = [];
  var y3 = [];
  var y4 = [];
  var y5 = [];

  for (var r = 0; r < data.length; r++) {
    x.push(data[r][0]);
    y1.push(data[r][1]);
    y2.push(data[r][2]);
    y3.push(data[r][3]);
    y4.push(data[r][4]);
    y5.push(data[r][5]);
  }

  var trace1 = {
    x: x,
    y: y1,
    mode: 'lines+markers',
    name: 'Processes',
    line: { dash: 'solid', width: 4 },
    marker: { size: 6 },
  };

  var trace2 = {
    x: x,
    y: y3,
    mode: 'lines+markers',
    name: 'Threads',
    line: { dash: 'solid', width: 4 },
    marker: { size: 6 },
  };

  var trace3 = {
    x: x,
    y: y5,
    mode: 'lines',
    name: 'Linear',
    line: { dash: 'dot', width: 4 },
  };

  var trace4 = {
    x: x,
    y: y2,
    yaxis: 'y2',
    mode: 'lines+markers',
    name: 'Process idle',
    line: { dash: 'solid', width: 2 },
    marker: { size: 8 },
  };

  var trace5 = {
    x: x,
    y: y4,
    yaxis: 'y2',
    mode: 'lines+markers',
    name: 'Thread idle',
    line: { dash: 'solid', width: 2 },
    marker: { size: 8 },
  };

  var graphs = [trace1, trace2, trace3, trace4, trace5];
  var layout = {
    title: title,
    xaxis: { title: 'Number of tasks' },
    yaxis: { title: 'Operations per second' },
    yaxis2: { title: 'Idle', side: 'right', overlaying: 'y', showline: true },
  };

  Plotly.newPlot('plot', graphs, layout);
}
