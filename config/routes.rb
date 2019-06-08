Rails.application.routes.draw do
  root "channels#index"

  resources :channels, only: [:index]
  resources :admin,    only: [:index]

  devise_for :users, controllers: {
    sessions:      'users/sessions',
    registrations: 'users/registrations',
  }
end
