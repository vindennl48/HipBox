App.Login = {
  SetUser: (name) => {
    document.getElementById('email').value        = `${name}@lof.com`
    document.getElementById('password').value     = 'password'
    document.getElementById('loginButton').hidden = false
  },

  Submit: () => {
    document.getElementById('submit').click()
  }
}
