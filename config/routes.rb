Rails.application.routes.draw do
  root "channels#index"

  resources :channels,    only: [:index]
  resources :admin,       only: [:index]
  resources :ports,       only: [:index, :new, :edit, :update, :destroy]
  resources :port_groups, only: [:new, :edit, :update, :destroy]
  resources :wifis,       only: [:index, :new, :edit, :update, :destroy]

  devise_for :users, controllers: {
    sessions:      'users/sessions',
    registrations: 'users/registrations',
  }
end
