import React, { useState, useEffect } from 'react'
import { Input, Button, Typography, Box, FormHelperText } from '@mui/joy/';
import { NavBar } from '../../components';



const Reciever = () => {
  const [textInputValue, setTextInputValue] = useState("");
  const [isRx, setRx] = useState(false);
  const [isLoading, setIsLoading] = useState(true)

  const handleTextInputChange = (event) => {
    setTextInputValue(event.target.value);
  };



  async function loadData() {
    try {
      const response = await fetch('http://localhost:5000/isRxRunning')
      const data = await response.json()
      console.log("data: " + data.isRx)
      setRx(data.isRx)
      setIsLoading(false)
    } catch (error) {
      console.error('Error fetching data:', error)
    }
  }
  useEffect(() => {
    loadData();
  }, []);

  const stopRx = async (event) => {
    setIsLoading(true)
    fetch('http://localhost:5000/stopRX', { method: 'POST' })
      .then(res => res.json())
      .then(data => {
        if (data.success) {

        }
      });
    loadData()
  }

  const handleSubmit = async (event) => {
    event.preventDefault();
    //alert("if you didn't fill the destination as per the format it will be saved on the project's directory!")
    var obj = {
      attributes: {
        srcPath: "textInputValue",
      }
    }
    obj.attributes.srcPath = textInputValue
    const a = obj
    const body = { a }
    const response = await fetch('http://localhost:5000/Receiver', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });

    const res = await response.json()
    console.log(res);

    loadData()
  };
  return (
    <div>
      <NavBar />
      <Box className="container" sx={{ display: 'flex', justifyContent: 'center', width: '50%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto' }}>
        <Typography gutterBottom level='h1' textColor={"common.white"}>
          Welcome to the Reciever configuration Settings
        </Typography>
        <Typography gutterBottom level='h1' textColor={"common.white"}>
          Please Enter your destination directory
        </Typography>

        <form onSubmit={(event) => {
          event.preventDefault();
          handleSubmit(event);
        }}>
          <Input
            required
            //className="input"
            placeholder='Enter destination path in here...'
            id="text-input"
            label="Text input"
            size='lg'

            variant='soft'

            value={textInputValue}
            onChange={handleTextInputChange}
            margin="normal"
          />
          <FormHelperText>an example for a path could be: /home/jonny/Desktop/project/</FormHelperText>

          <Button
            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
            loading={isLoading}
            type='submit'
            color="success">
            Start Transmission!
          </Button>
          <Button
            disabled={!isRx}
            loading={isLoading}
            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
            onClick={stopRx}
            color="danger">
            Stop Transmission!
          </Button>
        </form>
      </Box>
    </div>


  );
}

export default Reciever



