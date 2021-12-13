const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Web controlled LED</title>
  <style>
    #button {
      cursor: pointer;
      padding: 20px 40px;
      font-size: 20px;
      border-radius: 32px;
    }
    
    #center {
      text-align: center;
    }
    
    .css_on {
      background-color: green;
      color: white;
    }
    
    .css_off {
      background-color: red;
      color: black;
    }
  </style>
</head>
<body>
  <div class="center">
    <button onclick="postData()" id="button" class="css_off">LED off</button>
  </div>
  
  <script>
    let myData = { state: 'OFF' };
    let host = 'localhost';
  
    async function postData() {
    
      const button = document.getElementById('button');
      button.classList.remove('css_off', 'css_on');
    
      //   datalast
      if (myData.state == 'OFF') {
        myData = { state: 'ON' };
      } else {
        myData = { state: 'OFF' };
      }
    
    
      //https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
      const options = {
        method: 'POST',
        mode: 'cors',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(myData)
      }
    
      const response = await fetch('/api', options);
      
      //Hämta respons från server
      const jsonData = await response.json();
      console.log(jsonData);
    
      if (jsonData.state == 'ON') {
        button.classList.add('css_on');
        button.innerHTML = 'LED ON';
      } else {
        button.classList.add('css_off');
        button.innerHTML = 'LED OFF';
      }
    }
  </script>
</body>
</html>
)====";
