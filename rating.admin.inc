<?php
/**
*Form constructor for the rating_admin_form()
*/
function rating_admin_form($form)
{

$header = array(
    t('Name'),
    t('Rating'),
    t('Comments'),
    
    array('data' => t('Status'), 'colspan' => 1),
    array('data' => t('Operations'), 'colspan' => 2),
  );
$rows = array();

$ratings = db_select('myrating', 'r')
    ->fields('r', array('rid', 'name', 'rating', 'comment' , 'status'))
    ->orderBy('rid', 'DESC')
    ->execute()
    ->fetchAll();
foreach ($ratings as $rating) {
    $rows[] = array(
      check_plain($rating->name),
      check_plain($rating->rating),
      check_plain($rating->comment),
      ($rating->status ? t('Approved').l('/Unapprove','rating/'. $rating->rid.'/Unapprove') : t('Unapproved').l('/Approve','rating/'. $rating->rid.'/approve')),
      l(t('Edit'), 'rating/'. $rating->rid.'/edit'),
      l(t('Delete'), 'rating/'.$rating->rid.'/delete'),
    );
  }

if (!$rows) {
    $rows[] = array(array(
      'data' => t('No ratings available.'),
      'colspan' => 6,
    ));
  }

  $build['display'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  $build['pager'] = array('#theme' => 'pager');
  return $build;

}

/**
*Form constructor for the rating_edit_form()
*/
function rating_edit_form($form,&$form_submit,array $rating = array()) {

  $rating += array(
    'name' => '',
    'rating' => '',
    'comment' => '',
    'status' => 0,
    'rid' => NULL,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#maxlength' => 255,
    '#default_value' => $rating['name'],
    '#required' => TRUE,
  );
  $form['rating'] = array(
    '#type' => 'select',
    '#title' => t('Rating'),
    '#options' => array(
    '' => '', 
      '1' => '1',
      '1.5' => '1.5',
      '2' => '2',
      '2.5' => '2.5',
      '3' => '3',
      '3.5' => '3.5',
      '4' => '4',
      '4.5' => '4.5',
      '5' => '5',
    ),
    '#default_value' => (int)$rating['rating'],
     '#required' => TRUE,
     '#style' => 'display:inline',
  );
  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comments'),
    '#default_value' => $rating['comment'],
    '#required' => TRUE,
  );
  
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      0 => t('Not Approved'),
      1 => t('Approved'),
    ),
    '#default_value' => $rating['status'],
    
  );
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $rating['rid'],
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
*Form submission handler for the rating_edit_form()
*/
function rating_edit_form_submit($form,&$form_submit) 
{
 $name = check_plain($form_submit['values']['name']);
   $rating = check_plain($form_submit['values']['rating']);
   $comments = check_plain($form_submit['values']['comment']);
   $status = check_plain($form_submit['values']['status']);
   $rid = check_plain($form_submit['values']['rid']);
   $result= db_update('myrating')->fields(array(
  'name' => $name,
  'rating' =>$rating,
  'comment' => $comments,
  'status' => $status,))
  ->condition('rid',$rid,'=')
  ->execute();
  if ($result !== FALSE)
  {
    drupal_set_message('rating has been updated successfully.');
  }
$form_submit['redirect'] = 'admin/config/content/rating';
}

/**
*Form constructor for the rating_delete_form()
*/
function rating_delete_form($form,&$form_submit,array $rating)
{
$form['rating'] = array(
    '#type' => 'value',
    '#value' => $rating,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete customer %name rating?', array('%name' => $rating['name'])),
    'admin/config/content/rating',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
*Form submission handler for the rating_delete_form()
*/
function rating_delete_form_submit($form,&$form_submit)
{
$rating = $form['rating']['#value'];

  db_delete('myrating')
    ->condition('rid', $rating['rid'])
    ->execute();

  drupal_set_message(t(' %name rating has been deleted.', array('%name' => $rating['name'])));
 

  $form_submit['redirect'] = 'admin/config/content/rating';
}

function rating_approve($rid)
{
  $result= db_update('myrating')->fields(array(
    'status' => 1,))
  ->condition('rid',$rid['rid'],'=')
  ->execute();
  if ($result !== FALSE)
  {
     drupal_goto('admin/config/content/rating');
    drupal_set_message('rating has been approved.');
  }
 
}

function rating_unapprove($rid)
{
  echo $rid['rid'];
  $result= db_update('myrating')->fields(array(
    'status' => 0,))
  ->condition('rid',$rid['rid'],'=')
  ->execute();
  if ($result !== FALSE)
  {
    drupal_goto('admin/config/content/rating');
    drupal_set_message('rating has been unapproved.');
  }
  
}