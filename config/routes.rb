Rails.application.routes.draw do
  root "channels#index"

  resources :channels,    only: [:index]
  resources :admin,       only: [:index]
  resources :ports,       only: [:index, :edit, :update]
  resources :port_groups, only: [:edit, :update]

  devise_for :users, controllers: {
    sessions:      'users/sessions',
    registrations: 'users/registrations',
  }
end
