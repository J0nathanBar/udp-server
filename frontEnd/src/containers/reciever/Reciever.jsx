import React, { useState } from 'react'
import { Input, Button, Typography, Box, FormHelperText } from '@mui/joy/';
import { NavBar } from '../../components';



const Reciever = () => {
  const [textInputValue, setTextInputValue] = useState("");

  const handleTextInputChange = (event) => {
    setTextInputValue(event.target.value);
  };
  const handleSubmit = async (event) => {
    event.preventDefault();

    console.log("Text input value:", textInputValue);
    var obj = {
      attributes: {
        srcPath: "textInputValue",
      }
    }
    obj.attributes.srcPath = textInputValue
    const a = obj
    const body = { a }
    const response = await fetch('http://localhost:5000/Reciever', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });
    const res = await response.json()
    console.log(res);
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
            type='submit'
            className="button"
            variant="contained"
            // onClick={handleSubmit}
            sx={{ backgroundColor: '#3f51b5' }}
          >
            Submit
          </Button>
        </form>
      </Box>
    </div>


  );
}

export default Reciever



